#!/bin/sh
# Code/data analysis of Pac-Mania with SuperSpider (~/stuff/superspider):
# pacmania.json (the program, &8000) and playercode.json (the music and
# effects player it loads to &6F4B4 and calls through its branch table),
# which recomp/'s "make regen" translates to C with superspider gen.
# pacmania.roots adds entry points to the program's analysis (recomp/: "make
# roots" merges the gap entries a --trace-gaps run logged into it);
# playercode.roots holds the player's five entry points.
set -e
HERE="$(cd "$(dirname "$0")" && pwd)"
SUPERSPIDER="${SUPERSPIDER:-$HOME/stuff/superspider}"
SS="$SUPERSPIDER/_build/default/bin/main.exe"
APP="$HERE/../extracted/!PacMania"
(cd "$SUPERSPIDER" && dune build)
"$SS" analyse "$APP/PACMANIA,fffff843-2bb2e27b" 8000 --full --roots "$HERE/pacmania.roots" -o "$HERE/pacmania.json"
"$SS" analyse "$APP/PLAYERCODE,fffff843-21dc9687" 6f4b4 --full --roots "$HERE/playercode.roots" -o "$HERE/playercode.json"
