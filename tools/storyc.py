#!/usr/bin/env python3
"""Compiles content/ into one Lua module and refuses anything that does not hold together.

Dialogue (.dlg) becomes flat operation lists; cards, recipes and maps (.toml) become tables.
The checks here are the ones that need no play: every reference resolves, every speaker is
registered, every condition parses. Reachability and logic live in tools/story_check.py.
"""
import pathlib
import re
import sys
import tomllib

NODE_RE = re.compile(r"^=== *([a-z][a-z0-9_]*) *$")
SAY_RE = re.compile(r"^([^\s:：~*#=\->][^:：]{0,15})[:：] ?(.+)$")
ID_RE = re.compile(r"^[a-z][a-z0-9_]*$")
VAR_RE = re.compile(r"^var:([a-z][a-z0-9_]*)(>=|<=|==|!=|>|<)(-?\d+)$")
TERMINALS = ("END",)
CARD_KINDS = {"fact", "rule", "inference", "testimony", "fallacy"}
LOGIC_KINDS = {"deduction", "induction", "rebut", "fallacy"}


class ContentError(Exception):
    pass


def fail(path, line, message):
    raise ContentError(f"{path}:{line}: {message}")


def parse_cond(text, path, line):
    tokens = text.replace("(", " ( ").replace(")", " ) ").split()
    pos = 0

    def expr():
        nonlocal pos
        left = term()
        while pos < len(tokens) and tokens[pos] == "or":
            pos += 1
            left = ["or", left, term()]
        return left

    def term():
        nonlocal pos
        left = factor()
        while pos < len(tokens) and tokens[pos] == "and":
            pos += 1
            left = ["and", left, factor()]
        return left

    def factor():
        nonlocal pos
        if pos >= len(tokens):
            fail(path, line, f"condition ends early: {text}")
        tok = tokens[pos]
        pos += 1
        if tok == "not":
            return ["not", factor()]
        if tok == "(":
            inner = expr()
            if pos >= len(tokens) or tokens[pos] != ")":
                fail(path, line, f"missing ): {text}")
            pos += 1
            return inner
        match = VAR_RE.match(tok)
        if match:
            return ["var", match.group(1), match.group(2), int(match.group(3))]
        if ":" in tok:
            kind, name = tok.split(":", 1)
            if kind in ("has", "flag", "seen", "made") and ID_RE.match(name):
                return [kind, name]
        fail(path, line, f"bad condition term: {tok}")

    result = expr()
    if pos != len(tokens):
        fail(path, line, f"trailing condition text: {text}")
    return result


def parse_effect(text, path, line):
    parts = text.split()
    op = parts[0]
    if op in ("card", "flag", "clear", "ending") and len(parts) == 2 and ID_RE.match(parts[1]):
        return [op, parts[1]]
    if op in ("add", "set") and len(parts) == 3 and ID_RE.match(parts[1]):
        return [op, parts[1], int(parts[2])]
    if op in ("time", "chapter") and len(parts) == 2:
        return [op, int(parts[1])]
    if op == "fund" and len(parts) == 1:
        return [op]
    fail(path, line, f"bad effect: {text}")


# Words the period cannot say, and words the interface must not. See docs/story/01-bible.md.
BANNED = ("统计", "概率", "科学家", "细菌", "病毒", "免疫", "警察", "侦探", "旧历", "新历", "提示", "法医",
          "疫苗", "精算", "保险公司", "OK", "《人类理解研究》", "弓街探子", "贫民窟")
MAX_LINE_CHARS = 60
TEXT_VARS = ("fund_ruth", "fund_each", "fund_shares", "fund_total")


def lint_line(words, path, number):
    for word in BANNED:
        if word in words:
            fail(path, number, f"banned word {word}: {words}")
    if len(words) > MAX_LINE_CHARS:
        fail(path, number, f"line of {len(words)} chars, split it: {words}")
    if "不是" in words and "而是" in words:
        fail(path, number, f"no 不是……而是 constructions: {words}")
    for name in re.findall(r"\{([^}]*)\}", words):
        if name not in TEXT_VARS:
            fail(path, number, f"unknown text variable {name}")


class Lines:
    def __init__(self, path, text):
        self.path = path
        self.items = []
        for number, raw in enumerate(text.splitlines(), 1):
            stripped = raw.strip()
            if not stripped or stripped.startswith("#"):
                continue
            if "\t" in raw:
                fail(path, number, "tabs are not allowed")
            self.items.append((number, len(raw) - len(raw.lstrip(" ")), stripped))
        self.pos = 0

    def peek(self):
        return self.items[self.pos] if self.pos < len(self.items) else None

    def next(self):
        item = self.items[self.pos]
        self.pos += 1
        return item


class NodeBuilder:
    def __init__(self, node_id):
        self.node_id = node_id
        self.ops = []

    def emit(self, op):
        self.ops.append(op)
        return len(self.ops)  # 1-based pc of this op

    def here(self):
        return len(self.ops) + 1


def parse_block(lines, node, indent, stop_words, ctx):
    """Parses statements at this indent until a stop word, a node header or a dedent."""
    while True:
        item = lines.peek()
        if item is None:
            return None
        number, level, text = item
        word = text.split()[0]
        if word in stop_words:
            return word
        if text.startswith("===") or level < indent:
            return None
        if level != indent:
            fail(lines.path, number, f"unexpected indent {level}, expected {indent}")
        lines.next()
        if text.startswith("* "):
            lines.pos -= 1
            parse_choices(lines, node, indent, ctx)
        elif text.startswith("~ "):
            node.emit(["do", parse_effect(text[2:], lines.path, number)])
        elif text.startswith("-> "):
            target = text[3:].strip()
            if target == "END":
                node.emit(["end"])
            else:
                if not ID_RE.match(target):
                    fail(lines.path, number, f"bad target {target}")
                ctx["targets"].append((target, lines.path, number))
                node.emit(["goto", target])
        elif word == "if":
            parse_if(lines, node, indent, text[2:].strip(), number, ctx)
        else:
            match = SAY_RE.match(text)
            if not match:
                fail(lines.path, number, f"cannot read line: {text}")
            speaker, words = match.group(1).strip(), match.group(2).strip()
            if speaker not in ctx["speakers"]:
                fail(lines.path, number, f"unregistered speaker: {speaker}")
            lint_line(words, lines.path, number)
            node.emit(["say", "" if speaker == "旁白" else speaker, words])
            ctx["lines"] += 1
            ctx["chars"] += len(words)


def parse_if(lines, node, indent, cond_text, number, ctx):
    exits = []
    cond = parse_cond(cond_text, lines.path, number)
    while True:
        branch = node.emit(["jf", cond, 0])
        stop = parse_block(lines, node, indent + 2, ("elif", "else", "end"), ctx)
        if stop is None:
            fail(lines.path, number, "if without end")
        exits.append(node.emit(["jmp", 0]))
        node.ops[branch - 1][2] = node.here()
        number, _, text = lines.next()
        if stop == "elif":
            cond = parse_cond(text[4:].strip(), lines.path, number)
            continue
        if stop == "else":
            stop = parse_block(lines, node, indent + 2, ("end",), ctx)
            if stop is None:
                fail(lines.path, number, "else without end")
            lines.next()
        break
    for pc in exits:
        node.ops[pc - 1][1] = node.here()


OPTION_RE = re.compile(r"^\* (?:\(([^)]*)\) )?(.+)$")


def parse_choices(lines, node, indent, ctx):
    options = []
    choice_pc = node.emit(["choice", options, 0])
    exits = []
    while True:
        item = lines.peek()
        if item is None:
            break
        number, level, text = item
        if level != indent or not text.startswith("* "):
            break
        lines.next()
        match = OPTION_RE.match(text)
        if not match:
            fail(lines.path, number, f"bad option: {text}")
        spec, label = match.group(1), match.group(2).strip()
        once = None
        cond = None
        if spec:
            terms = [t.strip() for t in spec.split(",")]
            rest = [t for t in terms if t != "once"]
            if "once" in terms:
                once = f"{node.node_id}_{len(options) + 1}"
            if rest:
                if len(rest) > 1:
                    fail(lines.path, number, "one condition per option; join with and/or")
                cond = parse_cond(rest[0], lines.path, number)
        lint_line(label, lines.path, number)
        option = {"text": label, "pc": node.here()}
        if cond is not None:
            option["cond"] = cond
        if once is not None:
            option["once"] = once
        options.append(option)
        ctx["choices"] += 1
        parse_block(lines, node, indent + 2, (), ctx)
        exits.append(node.emit(["jmp", 0]))
    if not options:
        fail(lines.path, 0, "empty choice")
    node.ops[choice_pc - 1][2] = node.here()
    for pc in exits:
        node.ops[pc - 1][1] = node.here()


def parse_dialogue(path, ctx):
    text = path.read_text(encoding="utf-8")
    lines = Lines(str(path), text)
    while lines.peek() is not None:
        number, level, line = lines.next()
        match = NODE_RE.match(line)
        if not match or level != 0:
            fail(lines.path, number, f"expected a node header, got: {line}")
        node_id = match.group(1)
        if node_id in ctx["nodes"]:
            fail(lines.path, number, f"duplicate node {node_id}")
        node = NodeBuilder(node_id)
        stop = parse_block(lines, node, 0, (), ctx)
        if stop is not None:
            fail(lines.path, number, f"stray {stop}")
        node.emit(["end"])
        ctx["nodes"][node_id] = node.ops


def lua_value(value, depth=0):
    if isinstance(value, bool):
        return "true" if value else "false"
    if isinstance(value, int):
        return str(value)
    if isinstance(value, str):
        escaped = value.replace("\\", "\\\\").replace('"', '\\"').replace("\n", "\\n")
        return f'"{escaped}"'
    if isinstance(value, list):
        return "{" + ",".join(lua_value(v, depth + 1) for v in value) + "}"
    if isinstance(value, dict):
        items = []
        for key in sorted(value):
            if re.match(r"^[A-Za-z_][A-Za-z0-9_]*$", key) and key not in ("and", "or", "not", "end", "goto"):
                items.append(f"{key}={lua_value(value[key], depth + 1)}")
            else:
                items.append(f"[{lua_value(key)}]={lua_value(value[key], depth + 1)}")
        return "{" + ",".join(items) + "}"
    raise TypeError(type(value))


def load_chapters(root, ctx, only=None):
    """Chapter plans: budget, spots the player may visit, conclusions that must be reachable."""
    chapters = []
    for path in sorted((root / "story").glob("ch*.toml")):
        if only is not None and path.stem != f"ch{only}":
            continue
        data = tomllib.loads(path.read_text(encoding="utf-8"))
        for field in ("n", "title", "dates", "slots", "start", "climax"):
            if field not in data:
                raise ContentError(f"{path}: no {field}")
        ctx["targets"].append((data["start"], str(path), 0))
        ctx["targets"].append((data["climax"], str(path), 0))
        spots = []
        for spot in data.get("spot", []):
            entry = dict(spot)
            for field in ("id", "name", "place", "talk"):
                if field not in entry:
                    raise ContentError(f"{path}: spot without {field}: {spot}")
            if "cond" in entry:
                entry["cond"] = parse_cond(entry["cond"], str(path), 0)
            ctx["targets"].append((entry["talk"], str(path), 0))
            spots.append(entry)
        data["spot"] = spots
        forced = []
        for scene in data.get("forced", []):
            entry = dict(scene)
            ctx["targets"].append((entry["talk"], str(path), 0))
            if "cond" in entry:
                entry["cond"] = parse_cond(entry["cond"], str(path), 0)
            forced.append(entry)
        data["forced"] = forced
        chapters.append(data)
    chapters.sort(key=lambda c: c["n"])
    return chapters


VERBS = {"deduce", "induce", "rebut"}
FAULTS = {"肯定后件", "否定前件", "以偏概全", "相关当因果", "诉诸来源", "此地当各地"}


def field_missing(recipe, field):
    return field not in recipe


def check_recipe(path, recipe, tables):
    rid = recipe.get("id", "?")
    verb = recipe.get("verb")
    if verb not in VERBS:
        raise ContentError(f"{path}: {rid} has bad verb {verb}")
    if verb == "deduce":
        for field in ("major", "minor"):
            if field not in recipe:
                raise ContentError(f"{path}: {rid} needs {field}")
    elif verb == "induce":
        if recipe.get("table") not in tables:
            raise ContentError(f"{path}: {rid} names unknown table {recipe.get('table')}")
        if recipe.get("column") not in tables[recipe["table"]]["columns"]:
            raise ContentError(f"{path}: {rid} names unknown column")
        if recipe.get("min", 3) < 2:
            raise ContentError(f"{path}: {rid} needs at least two rows")
    else:
        if not recipe.get("target") and not recipe.get("target_tag"):
            raise ContentError(f"{path}: {rid} needs target or target_tag")
        if field_missing(recipe, "counter"):
            raise ContentError(f"{path}: {rid} needs counter")
    if recipe.get("fallacy"):
        if recipe["fallacy"] not in FAULTS:
            raise ContentError(f"{path}: {rid} names unknown fallacy {recipe['fallacy']}")
        if not recipe.get("fault"):
            raise ContentError(f"{path}: fallacy {rid} needs fault text")
    if not recipe.get("explain"):
        raise ContentError(f"{path}: {rid} has no explain")


def load_cards(root, only=None):
    cards, recipes, tables = {}, [], {}
    files = sorted((root / "cards").glob("*.toml"))
    if only is not None:
        files = [p for p in files if p.stem == f"ch{only}"]
    for path in files:
        data = tomllib.loads(path.read_text(encoding="utf-8"))
        for card in data.get("card", []):
            if card["id"] in cards:
                raise ContentError(f"{path}: duplicate card {card['id']}")
            if card["kind"] not in CARD_KINDS:
                raise ContentError(f"{path}: bad kind on {card['id']}")
            for field in ("title", "text", "source"):
                if not card.get(field):
                    raise ContentError(f"{path}: {card['id']} has no {field}")
            if card["kind"] == "rule" and card.get("certainty") not in ("必然", "或然"):
                raise ContentError(f"{path}: rule {card['id']} needs certainty 必然 or 或然")
            cards[card["id"]] = card
        for table in data.get("table", []):
            if table["id"] in tables:
                raise ContentError(f"{path}: duplicate table {table['id']}")
            for row in table.get("row", []):
                if len(row["values"]) != len(table["columns"]):
                    raise ContentError(f"{path}: table {table['id']} row {row['card']} has the wrong width")
            tables[table["id"]] = table
    for path in files:
        data = tomllib.loads(path.read_text(encoding="utf-8"))
        for recipe in data.get("recipe", []):
            check_recipe(path, recipe, tables)
            if "cond" in recipe:
                recipe["cond"] = parse_cond(recipe["cond"], str(path), 0)
            if "effects" in recipe:
                recipe["effects"] = [parse_effect(e, str(path), 0) for e in recipe["effects"]]
            recipes.append(recipe)
    ids = [r["id"] for r in recipes]
    if len(ids) != len(set(ids)):
        raise ContentError("duplicate recipe id")
    return cards, recipes, tables


FLAG_BITS = {"block": 1}


def load_maps(root, ctx):
    maps = {}
    for path in sorted((root / "maps").glob("*.toml")):
        data = tomllib.loads(path.read_text(encoding="utf-8"))
        rows = [r for r in data["rows"].splitlines() if r]
        width = len(rows[0])
        if any(len(r) != width for r in rows):
            raise ContentError(f"{path}: rows differ in width")
        legend = {}
        for key, spec in data["legend"].items():
            legend[key] = {
                "ground": spec["ground"],
                "height": spec.get("height", 0),
                "wall": spec.get("wall", "none"),
                "prop": spec.get("prop", "none"),
                "region": spec.get("region", 0),
                "flags": spec.get("flags", 0),
            }
        for row in rows:
            for ch in row:
                if ch not in legend:
                    raise ContentError(f"{path}: {ch!r} is not in the legend")
        npcs = []
        for npc in data.get("npc", []):
            entry = dict(npc)
            if "cond" in entry:
                entry["cond"] = parse_cond(entry["cond"], str(path), 0)
            ctx["targets"].append((entry["talk"], str(path), 0))
            npcs.append(entry)
        maps[data["id"]] = {
            "id": data["id"], "title": data["title"], "w": width, "h": len(rows),
            "rows": rows, "legend": legend, "start": data["start"], "npcs": npcs,
        }
    return maps


def check_card_refs(ctx, cards, recipes, tables):
    def walk(value, where):
        if isinstance(value, list) and value:
            if value[0] in ("has", "card") and len(value) > 1 and value[1] not in cards \
                    and not foreign(value[1], ctx["only"]):
                raise ContentError(f"{where}: unknown card {value[1]}")
            for item in value:
                walk(item, where)
        elif isinstance(value, dict):
            for item in value.values():
                walk(item, where)

    tags = set()
    for card in cards.values():
        tags.update(card.get("tags", []))
    for node_id, ops in ctx["nodes"].items():
        walk(ops, node_id)
    for table in tables.values():
        for row in table.get("row", []):
            if row["card"] not in cards and not foreign(row["card"], ctx["only"]):
                raise ContentError(f"table {table['id']}: unknown card {row['card']}")
    for recipe in recipes:
        named = [recipe.get(k) for k in ("major", "target", "output") if recipe.get(k)]
        named += recipe.get("retires", [])
        for card in named:
            if card not in cards and not foreign(card, ctx["only"]):
                raise ContentError(f"recipe {recipe['id']}: unknown card {card}")
        for field in ("minor", "counter", "target_tag"):
            tag = recipe.get(field)
            if tag and tag not in tags and ctx["only"] is None:
                raise ContentError(f"recipe {recipe['id']}: no card carries tag {tag}")
        walk(recipe.get("cond"), recipe["id"])
        walk(recipe.get("effects"), recipe["id"])


def foreign(name, only):
    """In single-chapter mode, a reference to another chapter is left for the full build."""
    match = re.match(r"^(?:c|ch)(\d)_", name or "")
    return only is not None and match is not None and int(match.group(1)) != only


def load_cast(root, only=None):
    """Every chapter keeps its own cast file, so chapters can be written side by side."""
    speakers, mortal = [], {}
    files = [root / "cast.toml"] + sorted((root / "cast").glob("*.toml"))
    if only is not None:
        files = [root / "cast.toml", root / "cast" / f"ch{only}.toml"]
    for path in files:
        if path.exists():
            data = tomllib.loads(path.read_text(encoding="utf-8"))
            speakers += data.get("speakers", [])
            mortal.update(data.get("mortal", {}))
    return {"speakers": speakers, "mortal": mortal}


def compile_content(root, only=None):
    cast = load_cast(root, only)
    ctx = {"nodes": {}, "speakers": set(cast["speakers"]), "targets": [], "lines": 0, "chars": 0, "choices": 0,
           "only": only}
    for path in sorted((root / "dialogue").rglob("*.dlg")):
        if only is None or path.parent.name == f"ch{only}":
            parse_dialogue(path, ctx)
    cards, recipes, tables = load_cards(root, only)
    maps = load_maps(root, ctx)
    chapters = load_chapters(root, ctx, only) if (root / "story").is_dir() else []
    for target, path, number in ctx["targets"]:
        if target not in ctx["nodes"] and not foreign(target, only) and not (only and target.startswith("demo_")):
            raise ContentError(f"{path}:{number}: unknown node {target}")
    check_card_refs(ctx, cards, recipes, tables)
    return {"nodes": ctx["nodes"], "cards": cards, "recipes": recipes, "tables": tables,
            "maps": maps, "chapters": chapters}, ctx


def main(argv):
    only = None
    if len(argv) == 5 and argv[1] == "--only":
        only = int(argv[2])
        argv = [argv[0]] + argv[3:]
    if len(argv) != 3:
        print("usage: storyc.py [--only N] content-dir out.lua", file=sys.stderr)
        return 2
    try:
        data, ctx = compile_content(pathlib.Path(argv[1]), only)
    except (ContentError, KeyError, tomllib.TOMLDecodeError) as error:
        print(f"content error: {error}", file=sys.stderr)
        return 1
    text = "return " + lua_value(data) + "\n"
    out = pathlib.Path(argv[2])
    out.parent.mkdir(parents=True, exist_ok=True)
    if not out.exists() or out.read_text(encoding="utf-8") != text:
        out.write_text(text, encoding="utf-8")
    print(f"content: {len(data['nodes'])} nodes, {ctx['lines']} lines, {ctx['chars']} chars, "
          f"{ctx['choices']} choices, {len(data['cards'])} cards, {len(data['recipes'])} recipes")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
