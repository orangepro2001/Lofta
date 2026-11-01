#!/usr/bin/env python3
"""Rules that keep scripts deterministic: no ambient randomness or clocks, no unordered walks."""
import re
import sys

FORBIDDEN = [
    (re.compile(r"\bmath\.random\b"), "math.random; use core.rng"),
    (re.compile(r"\bos\."), "os library"),
    (re.compile(r"\bio\."), "io library"),
    (re.compile(r"(?<![.:\w])load\s*\("), "load"),
    (re.compile(r"\bdofile\b"), "dofile"),
    (re.compile(r"\bpairs\s*\("), "pairs has no fixed order; end the line with -- sorted below"),
]
ALLOWED_MARK = "-- sorted below"


def strip_strings_and_comments(line):
    line = re.sub(r'"(?:[^"\\]|\\.)*"', '""', line)
    line = re.sub(r"'(?:[^'\\]|\\.)*'", "''", line)
    return line.split("--", 1)[0]


def main(paths):
    bad = 0
    for path in paths:
        with open(path, encoding="utf-8") as f:
            for number, line in enumerate(f, 1):
                code = strip_strings_and_comments(line)
                for pattern, why in FORBIDDEN:
                    if pattern.search(code) and ALLOWED_MARK not in line:
                        print(f"{path}:{number}: {why}", file=sys.stderr)
                        bad += 1
    if bad == 0:
        print("script check passed")
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
