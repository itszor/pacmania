#!/bin/sh
# Build Pac-Mania from your own copy of the game:
#     ./build.sh PacMania      (the ArcFS archive of the application, or a .zip holding it)
# then: cd recomp && ./pacmania
# The steps are recipe.json's "build", run by SuperSpider's tools/weave.py.
set -e
HERE="$(cd "$(dirname "$0")" && pwd)"
SUPERSPIDER="${SUPERSPIDER:-$HOME/stuff/superspider}"
[ $# -gt 0 ] || { echo "usage: $0 PACMANIA-ARCHIVE | GAME.zip" >&2; exit 2; }
(cd "$SUPERSPIDER" && dune build)
WEAVE_RECIPES="$HERE" exec python3 -I "$SUPERSPIDER/tools/weave.py" build "$@"
