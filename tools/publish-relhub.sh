#!/usr/bin/env bash
# publish-relhub.sh - publishes a built Lofta.exe to the release-publish service. Runs on ssh AI,
# where the service and the app token live; tools/release.sh puts the file there and calls it.
#
#   publish-relhub.sh --file EXE --version NAME --code N [--channel beta] [--notes FILE] [--dry-run]
#
# The token is ~/ci/lofta/etc/relhub-token (tools/relhub-setup.sh). Never run this with bash -x.
# Idempotent: when the channel already offers this exact file nothing is uploaded. A version code
# that is not higher than the newest published one is refused before uploading.
set -euo pipefail
case ":$PATH:" in *":$HOME/.local/bin:"*) ;; *) [ -x "$HOME/.local/bin/relhub" ] && PATH="$HOME/.local/bin:$PATH" ;; esac

APP=lofta
PLATFORM=win-x64
CHANNEL=beta
URL="${RELHUB_URL:-http://127.0.0.1:18790/release}"
TOKEN_FILE="${LOFTA_CI_HOME:-$HOME/ci/lofta}/etc/relhub-token"
FILE=""
NAME=""
CODE=0
NOTES=""
DRY=0
# The service's signing key, the same one the program pins (src/update/update_win.c).
KEY="k4thZR+OVYSGAT/9j5F39csgGqTL0t3X6GYdNCmuRI8="

while [ $# -gt 0 ]; do
  case "$1" in
    --file) FILE="$2"; shift ;;
    --version) NAME="$2"; shift ;;
    --code) CODE="$2"; shift ;;
    --channel) CHANNEL="$2"; shift ;;
    --notes) NOTES="$2"; shift ;;
    --dry-run) DRY=1 ;;
    *) echo "unknown option: $1" >&2; exit 2 ;;
  esac
  shift
done

die() { echo "ERROR: $*" >&2; exit 1; }
command -v relhub >/dev/null || die "relhub is not installed here (run this on ssh AI)"
command -v jq >/dev/null || die "jq is missing"
[ -f "$FILE" ] || die "no package $FILE"
[ -n "$NAME" ] || die "no --version"
[ "$CODE" -gt 0 ] 2>/dev/null || die "a dev build (code 0) cannot be published"
[ -s "$TOKEN_FILE" ] || die "no publish token: $TOKEN_FILE missing (run tools/relhub-setup.sh once)"
TOKEN=$(cat "$TOKEN_FILE")
SHA=$(sha256sum "$FILE" | cut -d' ' -f1)
URL="${URL%/}"

now=$(relhub check -json -url "$URL" -keys "$KEY" -data /nonexistent -app "$APP" -platform "$PLATFORM" -channel "$CHANNEL" -code 0 2>/dev/null || true)
cur_sha=$(printf '%s' "$now" | jq -r '.manifest.sha256 // ""' 2>/dev/null || echo "")
if [ "$cur_sha" = "$SHA" ]; then
  echo "$APP/$PLATFORM $CHANNEL already offers $NAME ($CODE) with this exact file; nothing to do"
  exit 0
fi
newest=$({ relhub list -url "$URL" -token "$TOKEN" -data /nonexistent -app "$APP" -platform "$PLATFORM" -all 2>/dev/null || true; } |
  awk 'NR > 1 && $1 ~ /^[0-9]+$/ { if ($1 > m) m = $1 } END { print m + 0 }')
[ "$CODE" -gt "$newest" ] || die "version code $CODE ($NAME) is not higher than the newest published $newest: raise VERSION and rebuild"

notes_file=$(mktemp)
trap 'rm -f "$notes_file"' EXIT
if [ -n "$NOTES" ]; then
  cat "$NOTES" > "$notes_file"
else
  printf '遗失的十一日 %s\n' "$NAME" > "$notes_file"
fi

echo "publish $APP/$PLATFORM $NAME (code $CODE) to $CHANNEL at $URL"
echo "   sha256 $SHA; newest published $newest"
if [ "$DRY" = 1 ]; then
  echo "   dry run: nothing uploaded"
  exit 0
fi
relhub publish -url "$URL" -token "$TOKEN" -data /nonexistent -app "$APP" -platform "$PLATFORM" -file "$FILE" \
  -version "$NAME" -code "$CODE" -channel "$CHANNEL" -notes-file "$notes_file" -name "Lofta-$NAME-$PLATFORM.exe"
rc=0
relhub check -url "$URL" -keys "$KEY" -data /nonexistent -app "$APP" -platform "$PLATFORM" -channel "$CHANNEL" -code $((CODE - 1)) || rc=$?
[ "$rc" = 10 ] || [ "$rc" = 20 ] || die "published, but a client with code $((CODE - 1)) is not offered it (relhub check exit $rc)"
