#!/bin/sh
# Enforces the layer rules in CLAUDE.md: the core stays free of platform APIs, allocation,
# ambient randomness, clocks, floating point and text literals.
set -u
status=0

report() {
    echo "layer violation: $1" >&2
    echo "$2" >&2
    status=1
}

for f in src/core/*.c src/core/*.h; do
    code=$(sed -e 's://.*$::' -e 's:/\*.*\*/::g' "$f")

    bad=$(printf '%s\n' "$code" | grep -nE '^[[:space:]]*#[[:space:]]*include[[:space:]]*<' |
        grep -vE '<(string|stdint|stddef)\.h>')
    [ -n "$bad" ] && report "$f includes a forbidden system header" "$bad"

    bad=$(printf '%s\n' "$code" | grep -nE '\b(malloc|calloc|realloc|free|rand|srand|time|clock|printf|fopen)[[:space:]]*\(')
    [ -n "$bad" ] && report "$f calls a forbidden function" "$bad"

    bad=$(printf '%s\n' "$code" | grep -nE '\b(float|double)\b')
    [ -n "$bad" ] && report "$f uses floating point" "$bad"

    bad=$(printf '%s\n' "$code" | grep -nE '\b(VK_[A-Z]|WM_[A-Z]|HWND|SDL_|KEY_[A-Z])')
    [ -n "$bad" ] && report "$f uses raw input or platform names" "$bad"

    bad=$(LC_ALL=C grep -n '[^ -~	]' "$f")
    [ -n "$bad" ] && report "$f contains non-ASCII text" "$bad"
done

# Hosts reach the core through core_api.h alone; the rest of src/core is private.
for f in src/platform/*.c src/platform/*.h src/platform/*/*.c src/platform/*/*.h src/update/*.c src/update/*.h; do
    bad=$(grep -nE '#[[:space:]]*include[[:space:]]*"(state|render|gfx|palette|text|sprites|rng|path|iso|font)\.h"' "$f")
    [ -n "$bad" ] && report "$f includes a private core header" "$bad"
done

[ "$status" -eq 0 ] && echo "layer check passed"
exit "$status"
