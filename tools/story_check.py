#!/usr/bin/env python3
"""The machine half of the red team. Proves what can be proved about content/ without playing.

Static: every node, card and recipe is reachable; every required conclusion survives the loss of
any single source; no required conclusion rests on a fallacy; dates agree with calendar_facts.

Dynamic: bots play the whole game through a Python copy of the dialogue machine, thousands of
times, under different habits. Every run must reach an ending, no one may speak after their death,
and every line, option, card and ending must turn up in some run.

Exhaustive: chapter 7 runs for every combination of the flags earlier chapters can set; each
combination must reach at least one ending.

  story_check.py [--runs N] [--seed S] [--demo]
"""
import argparse
import itertools
import pathlib
import random
import re
import sys
import tomllib

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))
import calendar_facts  # noqa: E402
import storyc  # noqa: E402

ROOT = pathlib.Path(__file__).resolve().parent.parent
MAX_OPS = 20000
MAX_ACTIONS = 600

COMPARE = {
    ">=": lambda a, b: a >= b, "<=": lambda a, b: a <= b, ">": lambda a, b: a > b,
    "<": lambda a, b: a < b, "==": lambda a, b: a == b, "!=": lambda a, b: a != b,
}


class Problems:
    def __init__(self):
        self.items = []

    def add(self, kind, text):
        self.items.append((kind, text))

    def report(self):
        for kind, text in self.items:
            print(f"PROBLEM [{kind}] {text}")
        return len(self.items)


class State:
    def __init__(self):
        self.cards = []
        self.flags = set()
        self.vars = {}
        self.seen = set()
        self.picked = set()
        self.made = set()
        self.slot = 0
        self.chapter = 0
        self.ending = ""

    def copy(self):
        other = State()
        other.cards = list(self.cards)
        other.flags = set(self.flags)
        other.vars = dict(self.vars)
        other.seen = set(self.seen)
        other.picked = set(self.picked)
        other.made = set(self.made)
        other.slot, other.chapter, other.ending = self.slot, self.chapter, self.ending
        return other

    def test(self, c):
        if c is None:
            return True
        op = c[0]
        if op == "and":
            return self.test(c[1]) and self.test(c[2])
        if op == "or":
            return self.test(c[1]) or self.test(c[2])
        if op == "not":
            return not self.test(c[1])
        if op == "has":
            return c[1] in self.cards
        if op == "flag":
            return c[1] in self.flags
        if op == "seen":
            return c[1] in self.seen
        if op == "made":
            return c[1] in self.made
        if op == "var":
            return COMPARE[c[2]](self.vars.get(c[1], 0), c[3])
        raise ValueError(op)

    def apply(self, e):
        op = e[0]
        if op == "card":
            if e[1] not in self.cards:
                self.cards.append(e[1])
        elif op == "flag":
            self.flags.add(e[1])
        elif op == "clear":
            self.flags.discard(e[1])
        elif op == "add":
            self.vars[e[1]] = self.vars.get(e[1], 0) + e[2]
        elif op == "set":
            self.vars[e[1]] = e[2]
        elif op == "time":
            self.slot += e[1]
        elif op == "chapter":
            self.chapter = e[1]
            self.slot = 0
        elif op == "ending":
            self.ending = e[1]
        elif op == "fund":
            f = self.flags
            ruth, total, pounds = calendar_facts.fund(
                "lucy_alive" in f, "will_alive" in f, "kitty_alive" in f, "ned_alive" in f,
                "lyle_guilty" in f, "pereira_hanged" in f)
            self.vars.update({"fund_shares": ruth, "fund_total": total, "fund_ruth": pounds,
                              "fund_each": calendar_facts.FUND // total})
        else:
            raise ValueError(op)


class Game:
    def __init__(self, data, cast, problems):
        self.data = data
        self.nodes = data["nodes"]
        self.cards = data["cards"]
        self.recipes = data["recipes"]
        self.chapters = {c["n"]: c for c in data["chapters"]}
        self.problems = problems
        self.mortal = {}
        for name, text in cast.get("mortal", {}).items():
            self.mortal[name] = storyc.parse_cond(text, "cast.toml", 0)
        self.hit_nodes = set()
        self.hit_options = set()
        self.hit_cards = set()
        self.hit_recipes = set()
        self.hit_endings = {}

    def run_node(self, state, node, choose):
        """Runs a conversation to its end. choose(state, node, options) picks an option."""
        pc = 0
        steps = 0
        state.seen.add(node)
        self.hit_nodes.add(node)
        while True:
            steps += 1
            if steps > MAX_OPS:
                self.problems.add("loop", f"conversation at {node} does not end")
                return
            ops = self.nodes[node]
            if pc >= len(ops):
                return
            op = ops[pc]
            name = op[0]
            if name == "say":
                cond = self.mortal.get(op[1])
                if cond is not None and state.test(cond):
                    self.problems.add("dead", f"{op[1]} speaks in {node} after their death: {op[2][:24]}")
                pc += 1
            elif name == "do":
                state.apply(op[1])
                if op[1][0] == "card":
                    self.hit_cards.add(op[1][1])
                pc += 1
            elif name == "jf":
                pc = pc + 1 if state.test(op[1]) else op[2] - 1
            elif name == "jmp":
                pc = op[1] - 1
            elif name == "goto":
                node = op[1]
                pc = 0
                state.seen.add(node)
                self.hit_nodes.add(node)
            elif name == "end":
                return
            elif name == "choice":
                options = [o for o in op[1] if state.test(o.get("cond")) and o.get("once") not in state.picked]
                if not options:
                    pc = op[2] - 1
                    continue
                option = choose(state, node, options)
                if option.get("once"):
                    state.picked.add(option["once"])
                self.hit_options.add((node, option["text"]))
                pc = option["pc"] - 1
            else:
                raise ValueError(name)

    def has_tag(self, state, tag):
        return any(tag in self.cards[c].get("tags", ()) for c in state.cards)

    def recipe_ready(self, state, r):
        verb = r["verb"]
        if verb == "deduce":
            ready = r["major"] in state.cards and self.has_tag(state, r["minor"])
        elif verb == "rebut":
            if r.get("target_tag"):
                ready = any(r["target_tag"] in self.cards[c].get("tags", ()) for c in state.cards)
            else:
                ready = r["target"] in state.cards
            ready = ready and self.has_tag(state, r["counter"])
        else:
            rows = self.data["tables"][r["table"]]["row"]
            ready = sum(1 for row in rows if row["card"] in state.cards) >= r.get("min", 3)
        return ready and state.test(r.get("cond"))

    def available_recipes(self, state):
        return [r for r in self.recipes if r["id"] not in state.made and self.recipe_ready(state, r)]

    def combine(self, state, recipe):
        state.made.add(recipe["id"])
        self.hit_recipes.add(recipe["id"])
        if recipe.get("output") and recipe["output"] not in state.cards:
            state.cards.append(recipe["output"])
            self.hit_cards.add(recipe["output"])
        for card in recipe.get("consumes", []):
            state.flags.add("spent_" + card)
        for e in recipe.get("effects", []):
            state.apply(e)

    def play_chapter(self, state, n, habits, rng):
        chapter = self.chapters[n]
        state.chapter = n
        state.slot = 0
        choose = habits["choose"]
        self.run_node(state, chapter["start"], lambda s, node, o: choose(s, node, o, rng))
        actions = 0
        visited = set()
        fired = set()
        while state.slot < chapter["slots"]:
            for scene in chapter.get("forced", []):
                if scene["talk"] not in fired and state.slot >= scene["slot"] and state.test(scene.get("cond")):
                    fired.add(scene["talk"])
                    self.run_node(state, scene["talk"], lambda s, node, o: choose(s, node, o, rng))
            actions += 1
            if actions > MAX_ACTIONS:
                self.problems.add("stuck", f"chapter {n} never ran out of time")
                break
            if state.chapter != n:  # the chapter ended from inside a conversation
                return
            options = []
            for spot in chapter.get("spot", []):
                if spot.get("once") and spot["id"] in visited:
                    continue
                if state.test(spot.get("cond")):
                    options.append(("spot", spot))
            for recipe in self.available_recipes(state):
                options.append(("recipe", recipe))
            options.append(("stop", None))
            kind, item = habits["act"](state, options, rng, chapter)
            if kind == "stop":
                break
            if kind == "recipe":
                self.combine(state, item)
            else:
                visited.add(item["id"])
                self.run_node(state, item["talk"], lambda s, node, o: choose(s, node, o, rng))
        for scene in chapter.get("forced", []):  # a forced scene is never skipped by a short chapter
            if scene["talk"] not in fired and state.test(scene.get("cond")):
                fired.add(scene["talk"])
                self.run_node(state, scene["talk"], lambda s, node, o: choose(s, node, o, rng))
        if state.chapter == n:
            self.run_node(state, chapter["climax"], lambda s, node, o: choose(s, node, o, rng))

    def play(self, habits, rng, state=None, first=None):
        state = state or State()
        numbers = sorted(self.chapters)
        for n in numbers:
            if first is not None and n < first:
                continue
            self.play_chapter(state, n, habits, rng)
            if state.chapter == n and n != numbers[-1]:
                self.problems.add("stuck", f"the climax of chapter {n} did not set the next chapter")
                state.chapter = n + 1
            if state.ending:
                self.hit_endings.setdefault(state.ending, 0)
                self.hit_endings[state.ending] += 1
                break
        if not state.ending and numbers:
            self.problems.add("ending", "a run finished without an ending")
        return state


def any_choice(_state, _node, options, rng):
    return rng.choice(options)


def last_choice(_state, _node, options, _rng):
    return options[-1]


def act_random(_state, options, rng, _chapter):
    return rng.choice(options)


def act_skip(_state, _options, _rng, _chapter):
    return ("stop", None)


def act_thorough(_state, options, rng, _chapter):
    recipes = [o for o in options if o[0] == "recipe"]
    if recipes:
        return rng.choice(recipes)
    spots = [o for o in options if o[0] == "spot"]
    # Talking costs nothing, so a thorough player still has to decide they are done.
    if not spots or rng.random() < 0.04:
        return ("stop", None)
    return rng.choice(spots)


def act_no_fallacy(state, options, rng, chapter):
    recipes = [o for o in options if o[0] == "recipe" and not o[1].get("fallacy")]
    if recipes:
        return rng.choice(recipes)
    return act_random(state, [o for o in options if o[0] != "recipe"], rng, chapter)


HABITS = {
    "wanderer": {"choose": any_choice, "act": act_random},
    "thorough": {"choose": any_choice, "act": act_thorough},
    "careful": {"choose": any_choice, "act": act_no_fallacy},
    "skipper": {"choose": any_choice, "act": act_skip},
    "leaver": {"choose": last_choice, "act": act_random},
}


# Static analysis over an AND/OR graph of "what yields what". Conditions are read generously:
# anything not negated must be obtainable, and negations are assumed satisfiable.
def positive_needs(c, out):
    if c is None:
        return
    op = c[0]
    if op in ("and", "or"):
        positive_needs(c[1], out)
        positive_needs(c[2], out)
    elif op == "not":
        return
    elif op in ("has", "flag", "seen", "made"):
        out.add((op, c[1]))


def cond_holds(c, facts):
    if c is None:
        return True
    op = c[0]
    if op == "and":
        return cond_holds(c[1], facts) and cond_holds(c[2], facts)
    if op == "or":
        return cond_holds(c[1], facts) or cond_holds(c[2], facts)
    if op in ("not", "var"):
        return True
    return (op, c[1]) in facts


EDGE_CACHE = {}


def node_edges(nodes, node_id):
    if node_id not in EDGE_CACHE:
        EDGE_CACHE[node_id] = list(node_edges_uncached(nodes, node_id))
    return EDGE_CACHE[node_id]


def node_edges_uncached(nodes, node_id):
    """Yields (guard chain, kind, payload) for everything a node can do, ignoring order."""
    ops = nodes[node_id]
    guards = {}
    for i, op in enumerate(ops):
        if op[0] == "jf":
            for k in range(i + 1, op[2] - 1):
                guards.setdefault(k, []).append(op[1])
    for i, op in enumerate(ops):
        chain = guards.get(i, [])
        if op[0] == "do":
            yield chain, "effect", op[1]
        elif op[0] == "goto":
            yield chain, "goto", op[1]
        elif op[0] == "choice":
            for option in op[1]:
                yield chain + ([option["cond"]] if option.get("cond") else []), "option", (option["text"], option["pc"] - 1)


REACH_CACHE = {}


def reachable(game, removed=frozenset()):
    """Everything obtainable, with removed conversations or recipes taken away. Cached, because the
    single-point analysis asks the same question once per required card."""
    if removed not in REACH_CACHE:
        REACH_CACHE[removed] = reachable_uncached(game, removed)
    return REACH_CACHE[removed]


def reachable_uncached(game, removed):
    """Fixpoint of everything obtainable; removed holds nodes, or recipe ids, to pretend are gone."""
    facts = set()
    nodes = set()
    roots = []
    for chapter in game.data["chapters"]:
        roots.append(chapter["start"])
        roots.append(chapter["climax"])
        roots.extend(scene["talk"] for scene in chapter.get("forced", []))
    changed = True
    while changed:
        changed = False
        frontier = list(roots)
        for chapter in game.data["chapters"]:
            for spot in chapter.get("spot", []):
                if cond_holds(spot.get("cond"), facts):
                    frontier.append(spot["talk"])
        for node in frontier:
            if node in removed:
                continue
            if node not in nodes:
                nodes.add(node)
                facts.add(("seen", node))
                changed = True
        for node in list(nodes):
            for chain, kind, payload in node_edges(game.nodes, node):
                if not all(cond_holds(c, facts) for c in chain):
                    continue
                if kind == "goto" and payload not in removed and payload not in nodes:
                    nodes.add(payload)
                    facts.add(("seen", payload))
                    changed = True
                elif kind == "effect":
                    op = payload[0]
                    fact = ("has", payload[1]) if op == "card" else ("flag", payload[1]) if op == "flag" else None
                    if fact and fact not in facts:
                        facts.add(fact)
                        changed = True
        for recipe in game.recipes:
            if recipe["id"] in removed or ("made", recipe["id"]) in facts:
                continue
            if recipe_possible(game, recipe, facts) and cond_holds(recipe.get("cond"), facts):
                facts.add(("made", recipe["id"]))
                if recipe.get("output"):
                    facts.add(("has", recipe["output"]))
                for e in recipe.get("effects", []):
                    if e[0] == "flag":
                        facts.add(("flag", e[1]))
                    elif e[0] == "card":
                        facts.add(("has", e[1]))
                changed = True
    return nodes, facts


def recipe_possible(game, recipe, facts):
    held = {name for kind, name in facts if kind == "has"}

    def tagged(tag):
        return any(tag in game.cards[c].get("tags", ()) for c in held)

    verb = recipe["verb"]
    if verb == "deduce":
        return recipe["major"] in held and tagged(recipe["minor"])
    if verb == "rebut":
        if recipe.get("target_tag"):
            return tagged(recipe["target_tag"]) and tagged(recipe["counter"])
        return recipe["target"] in held and tagged(recipe["counter"])
    rows = game.data["tables"][recipe["table"]]["row"]
    return sum(1 for row in rows if row["card"] in held) >= recipe.get("min", 3)


def card_sources(game):
    """Which conversations and recipes hand out each card."""
    sources = {}
    for node in game.nodes:
        for op in game.nodes[node]:
            if op[0] == "do" and op[1][0] == "card":
                sources.setdefault(op[1][1], set()).add(node)
    for recipe in game.recipes:
        if recipe.get("output"):
            sources.setdefault(recipe["output"], set()).add(recipe["id"])
    return sources


def static_checks(game, problems):
    nodes, facts = reachable(game)
    for node in game.nodes:
        if node not in nodes and not node.startswith("demo_"):
            problems.add("reach", f"node {node} can never be reached")
    for card in game.cards:
        if ("has", card) not in facts and not card.startswith("c_demo"):
            problems.add("reach", f"card {card} can never be obtained")
    for recipe in game.recipes:
        if ("made", recipe["id"]) not in facts and not recipe["id"].startswith("d_demo"):
            problems.add("reach", f"recipe {recipe['id']} can never be made")

    sources = card_sources(game)
    for chapter in game.data["chapters"]:
        for card in chapter.get("required", []):
            if ("has", card) not in facts:
                problems.add("required", f"chapter {chapter['n']}: {card} is unreachable")
                continue
            kinds = [k for k in (game.cards[card]["kind"],)]
            if "fallacy" in kinds:
                problems.add("required", f"chapter {chapter['n']}: {card} is a fallacy")
            for source in sources.get(card, ()):
                # Taking away any one conversation or recipe must leave a way to the conclusion,
                # unless the conclusion comes from more than one source anyway.
                if len(sources[card]) > 1:
                    _, left = reachable(game, frozenset({source}))
                    if ("has", card) not in left:
                        problems.add("spof", f"chapter {chapter['n']}: {card} is lost without {source}")
            # Every premise of the deciding recipes must not be a single point of failure either.
            for removed in single_points(game, card, sources):
                problems.add("spof", f"chapter {chapter['n']}: {card} depends on {removed} alone")


def single_points(game, card, sources):
    """Conversations whose loss makes the card unobtainable. Openings and climaxes do not count:
    they are not optional. Chapters may mark a card as single_path to accept one by design."""
    if game.cards[card].get("single_path"):
        return []
    fixed = set()
    for chapter in game.data["chapters"]:
        fixed.add(chapter["start"])
        fixed.add(chapter["climax"])
        fixed.update(scene["talk"] for scene in chapter.get("forced", []))
    result = []
    for node in game.nodes:
        if node in fixed or node.startswith("demo_"):
            continue
        _, left = reachable(game, frozenset({node}))
        if ("has", card) not in left:
            result.append(node)
    return result


def check_dates(problems):
    bad = 0
    for text, style, year, month, day, expected in calendar_facts.FACTS:
        if calendar_facts.weekday(style, year, month, day) != expected:
            bad += 1
            problems.add("date", text)
    return bad


WEEKDAY_RE = re.compile(r"(\d{1,2})月(\d{1,2})日[，, ]*星期([一二三四五六日])")
WEEK_NAMES = {"一": 0, "二": 1, "三": 2, "四": 3, "五": 4, "六": 5, "日": 6}


def check_text_dates(game, problems):
    """Any "M月D日 星期X" in a line must be right for 1752, Old Style up to 2 September."""
    import datetime as dt
    for node, ops in game.nodes.items():
        for op in ops:
            if op[0] != "say":
                continue
            for month, day, week in WEEKDAY_RE.findall(op[2]):
                m, d = int(month), int(day)
                try:
                    date = dt.date(1752, m, d)
                except ValueError:
                    continue
                if (m, d) <= (9, 2):
                    date += calendar_facts.OFFSET
                if date.weekday() != WEEK_NAMES[week]:
                    problems.add("date", f"{node}: {m}月{d}日 is not 星期{week}: {op[2][:30]}")


def exhaustive_finale(game, problems):
    numbers = sorted(game.chapters)
    if not numbers or "finale" not in game.chapters[numbers[-1]]:
        return 0
    last = game.chapters[numbers[-1]]
    flags = last["finale"]["flags"]
    needed_endings = set(last["finale"].get("endings", []))
    seen_endings = set()
    combos = 0
    for values in itertools.product((False, True), repeat=len(flags)):
        combos += 1
        state = State()
        state.flags = {f for f, v in zip(flags, values) if v}
        reached = set()
        rng = random.Random(combos)
        for _ in range(last["finale"].get("tries", 12)):
            trial = state.copy()
            game.play_chapter(trial, last["n"], HABITS["careful"], rng)
            if trial.ending:
                reached.add(trial.ending)
        if len(reached) < last["finale"].get("min_endings", 1):
            problems.add("ending", f"only {sorted(reached)} for {sorted(state.flags)}")
        seen_endings |= reached
    for ending in needed_endings - seen_endings:
        problems.add("ending", f"ending {ending} is never reached in the finale check")
    print(f"finale: {combos} flag combinations, endings seen {sorted(seen_endings)}")
    return combos


BUDGET = {1: (12, 18, 8), 2: (14, 18, 8), 3: (18, 20, 9), 4: (12, 12, 5), 5: (12, 14, 7), 6: (16, 16, 9), 7: (6, 8, 4)}
SIX_THINGS = 6
MONEY_RE = re.compile(r"(\d[\d,]{3,})\s*镑")


def content_invariants(game, problems):
    """Budget limits, the six things Tuck says, flags that nothing reads, written-out fund sums."""
    tuck = [c for c, card in game.cards.items() if card.get("source") == "老塔克" and card.get("six")]
    if game.data["chapters"] and len(game.data["chapters"]) == 7 and len(tuck) != SIX_THINGS:
        problems.add("six", f"{len(tuck)} of the six things carry source 老塔克 and six = true")
    for chapter in game.data["chapters"]:
        n = chapter["n"]
        if n not in BUDGET:
            continue
        slots, cards, recipes = BUDGET[n]
        if chapter["slots"] > slots:
            problems.add("budget", f"chapter {n} has {chapter['slots']} slots, limit {slots}")
        count = sum(1 for c in game.cards.values() if c.get("chapter") == n and c["kind"] != "fallacy")
        if count > cards:
            problems.add("budget", f"chapter {n} has {count} cards, limit {cards}")
        made = sum(1 for r in game.recipes if r["id"][1:2] == str(n))
        if made > recipes:
            problems.add("budget", f"chapter {n} has {made} recipes, limit {recipes}")
    for node, ops in game.nodes.items():
        for op in ops:
            if op[0] == "say":
                for amount in MONEY_RE.findall(op[2]):
                    if amount.replace(",", "") != str(calendar_facts.FUND):
                        problems.add("money", f"{node}: write {amount} 镑 with fund variables: {op[2][:30]}")
    set_flags, read_flags = {}, set()

    def walk(value):
        if isinstance(value, list) and value:
            if value[0] == "flag" and len(value) == 2 and isinstance(value[1], str):
                read_flags.add(value[1])
            for item in value:
                walk(item)
        elif isinstance(value, dict):
            for item in value.values():
                walk(item)

    for node, ops in game.nodes.items():
        for op in ops:
            if op[0] == "do" and op[1][0] == "flag":
                set_flags.setdefault(op[1][1], node)
            elif op[0] in ("jf", "choice"):
                walk(op[1])
    for chapter in game.data["chapters"]:
        walk([s.get("cond") for s in chapter.get("spot", [])])
        walk([s.get("cond") for s in chapter.get("forced", [])])
        for flag in chapter.get("finale", {}).get("flags", []):
            read_flags.add(flag)
    for recipe in game.recipes:
        walk(recipe.get("cond"))
    for flag, node in set_flags.items():
        if flag not in read_flags and not flag.endswith("_done"):
            problems.add("flag", f"flag {flag} set in {node} is never read")


def coverage(game, problems, data):
    for node in game.nodes:
        if node not in game.hit_nodes and not node.startswith("demo_"):
            problems.add("coverage", f"no bot reached node {node}")
    for card in game.cards:
        if card not in game.hit_cards and not card.startswith("c_demo"):
            problems.add("coverage", f"no bot obtained card {card}")
    for recipe in game.recipes:
        if recipe["id"] not in game.hit_recipes and not recipe["id"].startswith("d_demo"):
            problems.add("coverage", f"no bot made recipe {recipe['id']}")
    total_options = 0
    missed = []
    for node, ops in game.nodes.items():
        if node.startswith("demo_"):
            continue
        for op in ops:
            if op[0] == "choice":
                for option in op[1]:
                    total_options += 1
                    if (node, option["text"]) not in game.hit_options:
                        missed.append((node, option["text"]))
    for node, text in missed:
        problems.add("coverage", f"no bot chose '{text}' in {node}")
    return total_options


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--runs", type=int, default=4000)
    ap.add_argument("--seed", type=int, default=1752)
    a = ap.parse_args()

    problems = Problems()
    root = ROOT / "content"
    try:
        data, ctx = storyc.compile_content(root)
    except (storyc.ContentError, KeyError, tomllib.TOMLDecodeError) as error:
        print(f"content error: {error}")
        return 1
    cast = storyc.load_cast(root)
    game = Game(data, cast, problems)

    check_dates(problems)
    check_text_dates(game, problems)
    if not data["chapters"]:
        print("no chapters yet: dates checked only")
        return 1 if problems.report() else 0

    static_checks(game, problems)
    content_invariants(game, problems)
    rng = random.Random(a.seed)
    runs = 0
    for name, habits in HABITS.items():
        for _ in range(a.runs):
            game.play(habits, rng)
            runs += 1
    combos = exhaustive_finale(game, problems)
    options = coverage(game, problems, data)
    unique = {}
    for kind, text in problems.items:
        unique[(kind, text)] = True
    problems.items = list(unique)
    count = problems.report()
    print(f"story check: {len(game.nodes)} nodes, {ctx['lines']} lines, {ctx['chars']} chars, {options} options, "
          f"{len(game.cards)} cards, {len(game.recipes)} recipes; {runs} runs, {combos} finale combinations; "
          f"endings {dict(sorted(game.hit_endings.items()))}; {count} problems")
    return 1 if count else 0


if __name__ == "__main__":
    sys.exit(main())
