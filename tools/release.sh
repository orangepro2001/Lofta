#!/usr/bin/env bash
# One command from this machine: test and build on egg, publish on AI.
#
#   tools/release.sh [--channel beta|stable] [--notes FILE] [--dry-run]
#
# The version comes from the VERSION file and only goes up. Default channel: beta.
set -euo pipefail
cd "$(dirname "${BASH_SOURCE[0]}")/.."

CHANNEL=beta
NOTES=""
DRY=""
while [ $# -gt 0 ]; do
  case "$1" in
    --channel) CHANNEL="$2"; shift ;;
    --notes) NOTES="$2"; shift ;;
    --dry-run) DRY="--dry-run" ;;
    *) echo "unknown option: $1" >&2; exit 2 ;;
  esac
  shift
done

NAME=$(tr -d ' \r\n' < VERSION)
IFS=. read -r MAJOR MINOR PATCH <<< "$NAME"
CODE=$((MAJOR * 10000 + MINOR * 100 + PATCH))

tools/egg.sh test
tools/egg.sh RELEASE=1 win
python3 tools/win_check.py crosscheck
python3 tools/win_check.py smoke build/egg/release/Lofta.exe

ssh AI 'mkdir -p ~/ci/lofta/incoming ~/ci/lofta/bin'
rsync -az tools/publish-relhub.sh AI:ci/lofta/bin/
rsync -az build/egg/release/Lofta.exe "AI:ci/lofta/incoming/Lofta-$NAME.exe"
if [ -n "$NOTES" ]; then
  rsync -az "$NOTES" "AI:ci/lofta/incoming/notes-$NAME.md"
  ssh AI "bash ci/lofta/bin/publish-relhub.sh --file ci/lofta/incoming/Lofta-$NAME.exe --version $NAME --code $CODE --channel $CHANNEL --notes ci/lofta/incoming/notes-$NAME.md $DRY"
else
  ssh AI "bash ci/lofta/bin/publish-relhub.sh --file ci/lofta/incoming/Lofta-$NAME.exe --version $NAME --code $CODE --channel $CHANNEL $DRY"
fi
