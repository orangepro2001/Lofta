#!/usr/bin/env bash
# Compiling happens on the egg server. Syncs this tree there, runs make with the given targets
# and brings build outputs back to build/egg/.
#
#   tools/egg.sh test win shots      any make targets
#   tools/egg.sh --fetch-only        only bring outputs back
set -euo pipefail
cd "$(dirname "${BASH_SOURCE[0]}")/.."

REMOTE=${LOFTA_REMOTE:-egg}
DIR=${LOFTA_REMOTE_DIR:-dev/game-dev/Lofta}

sync_up() {
    rsync -az --delete --exclude build/ --exclude '__pycache__/' --exclude '*.pyc' ./ "$REMOTE:$DIR/"
}

fetch() {
    mkdir -p build/egg
    rsync -az --prune-empty-dirs --include '*/' --include '*.exe' --include '*.png' --include '*.txt' --include '*.log' \
        --exclude '*' "$REMOTE:$DIR/build/" build/egg/
}

if [ "${1:-}" = "--fetch-only" ]; then
    fetch
    exit 0
fi

sync_up
status=0
ssh "$REMOTE" "cd '$DIR' && make -j2 $*" || status=$?
fetch
exit "$status"
