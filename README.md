Pac-Mania (Domark / Grandslam, 1991) - static recompilation
==========================================================

Archimedes conversion of Namco's Pac-Mania (Krisalis Software: Shaun
Hollingworth and Pete Harrap, ARM code; Jason Wilson, graphics; Benn
Daglish, music). Original: `PacMania`, an ArcFS archive of the application.

A recipe: this repository holds no game data. It rebuilds the game from
your own copy, recognised by content (recipe.json):

    ./build.sh PacMania          (the ArcFS archive, or a .zip holding it)
    cd recomp && ./pacmania

build.sh runs recipe.json's steps with SuperSpider's tools/weave.py
(~/stuff/superspider, or $SUPERSPIDER; $SS below): it unpacks the archive
to extracted/arc (!PacMania, and MonitorFix: an alternative !Run) and
!PacMania to game/ (libroger's appdir), then analyses, translates and
links. Needs python3, OCaml/dune, gcc and SDL3.

    analysis/run.sh                         code/data separation with SuperSpider
                                            (superspider analyse --full) -> pacmania.json,
                                            playercode.json
    recomp/                                 make regen (analysis + superspider gen), make,
                                            ./pacmania

What runs
---------

!Run RMLoads MemAlloc, sets `*ScreenSize 80` and `WimpSlot 620K`, loads
the Joystick module if it isn't there, then `Load Pacmania 8000 ; Go 8000`.

- `PACMANIA` (absolute, &8000): the game. All its buffers and pointers are
  relative to where it finds itself loaded, which it keeps in its workspace
  (`[r11+&34]`); its state machines jump through tables of offsets from that
  address.
- `PLAYERCODE` (absolute, loaded by the game to &6F4B4): the music and
  sound-effects player (it loads MDAT0-4 and EFFECTS and installs its own
  voice generators). The game calls it through a five-entry branch table at
  its start. Translated as a second image, an overlay in game.c.
- MemAlloc only provides `*ScreenSize` (80K is libroger's default screen
  memory). Joymod is a driver for a hardware joystick interface (SWI chunk
  &CFFC0): it isn't loaded, so the game's joystick SWI fails and it reads
  the keyboard, as it does on a machine without the interface.
- The game's `*RMReInit SoundChannels` / `WaveSynth` are logged and ignored.

SuperSpider needed two general additions (src/s1.ml) for this game, neither
of which changes the analysis of the other games:

- a hand-linked call through a pointer loaded from memory (`ldr r1,[r11,#k] ;
  add lr,pc,#0 ; mov pc,r1` - the calls into PLAYERCODE) is a call, not a
  return. Before this, everything after the first such call was lost (1594
  code words of the program instead of 6243);
- a table entry plus a field (`ldr r0,[r5,r0] ; ldr r1,[r11,#&34] ; add
  r0,r0,r1 ; mov pc,r0`): the targets are the entries plus the values the
  program stores at that field offset (`tfield`).

Result: the program 6243 code words, 173 functions; the player 1100 code
words, 61 functions. The player's roots file lists its branch table and a
case table (&6FB24-&6FB48) reached through a resume address stored after a
signed bounds check, which the analysis doesn't narrow (see the comment in
analysis/playercode.roots). Nothing is interpreted: no gaps showed in
--trace-gaps runs through the title, the theatre, attract mode, several
games, deaths, game over and the high-score entry.

Running
-------

    cd recomp && make && ./pacmania [--no-sound] [--scale=N] [--fullscreen] [--crt]
    ./pacmania --trace-gaps          interpret untranslated code instead of stopping, logging
                                     each new address to untranslated.log (make roots, make regen)
    ./pacmania --limit=50000000      headless: game.c's scenario starts a game and steers a little

Keys (the defaults from the KEYS file): Z left, X right, P up, L down,
Space bounce. On the title screen: 1 or Space starts, H high scores, Shift+D
defines keys, Shift+P pauses, Shift+S speaker, Shift+M music, Shift+Q quits
the game in play; the game says Ctrl+Escape returns to the desktop (not
tried here).
`--script` key numbers are the negated INKEY numbers (Space 99, Z 98, X 67,
P 56, L 87).

Not checked yet: the later worlds (Sandbox Land, Jungly Steps, the theatre
between them) beyond what --trace-gaps runs reached, the MonitorFix !Run
(a 70Hz 320x256 mode for monitors that can't show the standard modes - not
needed here), and what the music sounds like against the original.
