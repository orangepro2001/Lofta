#!/usr/bin/env bash
# Builds third_party/raylib-6.2/lib/libraylib.a from the official source with egg's mingw, so
# the library and the compiler that links it agree. Run once on egg; the result is vendored.
set -euo pipefail
VERSION=6.2
WORK=${1:-$HOME/.cache/lofta-raylib}
OUT=${2:-third_party/raylib-$VERSION/lib/libraylib.a}

mkdir -p "$WORK"
cd "$WORK"
[ -f "raylib-$VERSION.tar.gz" ] || curl -fsSL -o "raylib-$VERSION.tar.gz" "https://github.com/raysan5/raylib/archive/refs/tags/$VERSION.tar.gz"
sha256sum "raylib-$VERSION.tar.gz"
rm -rf "raylib-$VERSION"
tar xzf "raylib-$VERSION.tar.gz"
cd "raylib-$VERSION/src"
make -j2 PLATFORM=PLATFORM_DESKTOP_GLFW OS=Windows_NT CC=x86_64-w64-mingw32-gcc AR=x86_64-w64-mingw32-ar RAYLIB_LIBTYPE=STATIC >/dev/null
ls -la libraylib.a
cd - >/dev/null
