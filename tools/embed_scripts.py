#!/usr/bin/env python3
"""Compiles every Lua script into C byte arrays, so a release build stays one file.

Module names follow require: scripts/screens/world.lua becomes "screens.world".
"""
import pathlib
import sys


def main():
    out, root, gen, *paths = sys.argv[1:]
    bases = [pathlib.Path(root), pathlib.Path(gen)]
    parts = ['#include "lua_host.h"\n\n']
    table = []
    for index, name in enumerate(sorted(paths)):
        path = pathlib.Path(name)
        data = path.read_bytes()
        base = next(b for b in bases if b in path.parents)
        module = ".".join(path.relative_to(base).with_suffix("").parts)
        values = ",".join(str(b) for b in data)
        parts.append(f"static const char S{index}[] = {{{values}}};\n")
        table.append(f'    {{"{module}", S{index}, {len(data)}u}},\n')
    parts.append("\nconst EmbeddedScript EMBEDDED_SCRIPTS[] = {\n")
    parts.extend(table)
    parts.append("    {0, 0, 0u},\n};\n")
    pathlib.Path(out).write_text("".join(parts), encoding="utf-8")


if __name__ == "__main__":
    main()
