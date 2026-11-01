#!/usr/bin/env bash
# relhub-setup.sh - one-time preparation of the release-publish service (relhub) for Lofta. Run it on the
# build server (ssh AI), where relhub and its admin token live. Idempotent: an existing app or token is kept.
#
#   scripts/relhub-setup.sh            create the app "lofta" and an app-scoped publish token
#
# The token (it can publish, promote and withdraw Lofta releases, nothing else) is written once to
# $LOFTA_CI_HOME/etc/relhub-token (default ~/ci/lofta/etc/relhub-token, mode 600). It is a secret: never
# commit it, never print it. Lost or leaked: relhub token revoke -id <id>, delete the file, run this again.
set -euo pipefail
# relhub is installed per user (~/.local/bin), which non-interactive ssh shells do not have on PATH
case ":$PATH:" in *":$HOME/.local/bin:"*) ;; *) [ -x "$HOME/.local/bin/relhub" ] && PATH="$HOME/.local/bin:$PATH" ;; esac

APP=lofta
NAME="遗失的十一日"
DESC="The Eleven Lost Days: 1752 年伦敦的等距推理游戏（Windows）"
CI_HOME="${LOFTA_CI_HOME:-$HOME/ci/lofta}"
TOKEN_FILE="$CI_HOME/etc/relhub-token"
command -v relhub >/dev/null || { echo "ERROR: relhub is not installed here (run this on the build server)" >&2; exit 1; }

# admin calls: the local relhub CLI reads the admin token of the service's data directory
if relhub app show -id "$APP" >/dev/null 2>&1; then
  echo "app $APP exists"
else
  relhub app create -id "$APP" -name "$NAME" -desc "$DESC"
fi

umask 077
mkdir -p "$CI_HOME/etc"
if [ -s "$TOKEN_FILE" ]; then
  echo "token file exists: $TOKEN_FILE (kept)"
else
  out=$(relhub token create -name "ci-$APP" -app "$APP")
  printf '%s\n' "$out" | grep -o 'rl_[A-Za-z0-9_-]*' > "$TOKEN_FILE"
  chmod 600 "$TOKEN_FILE"
  [ -s "$TOKEN_FILE" ] || { echo "ERROR: no token in the answer of relhub token create" >&2; exit 1; }
  printf '%s\n' "$out" | grep -v 'rl_' || true
  echo "token stored in $TOKEN_FILE (600)"
fi
relhub token list | awk -v app="$APP" 'NR == 1 || $0 ~ app'
