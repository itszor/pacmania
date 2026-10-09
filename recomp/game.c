/* Pac-Mania (Domark / Grandslam, Archimedes conversion): what libroger needs
 * to know about the game.
 *
 * !Run: Dir <Obey$Dir> ; RMLoad MemAlloc ; WimpSlot 0 0 ; ScreenSize 80 ;
 * WimpSlot 620K ; RMEnsure Joystick 0 RMLoad Joymod ; Load Pacmania 8000 ;
 * Go 8000. The program loads its data by leaf name from the current
 * directory (the application directory), and PLAYERCODE - its music and
 * effects player - to &6F4B4, which it calls through the player's branch
 * table. MemAlloc only provides *ScreenSize (80K is libroger's default
 * screen memory). Joymod drives a hardware joystick interface: without it
 * the program's joystick SWI (&CFFC0) fails and it reads the keyboard. */
#include <roger.h>

extern const struct roger_image recomp_image;          /* gen/entries.c */
extern const struct roger_image recomp_player_image;   /* gen_player/entries.c */

static const struct roger_program programs[] = {
    {"PLAYERCODE", &recomp_player_image, 1},
    {NULL, NULL, 0},
};

static const char *const prefixes[] = {"<Obey$Dir>.", "<PacMania$Dir>.", NULL};

/* Headless scenario (--limit runs; 2000 instructions a millisecond): Space
 * past the title screen and the theatre, then a little steering with the
 * KEYS file's defaults - Z left, X right, P up, L down (negated INKEY
 * numbers: Space 99, Z 98, X 67, P 56, L 87). */
#define MS(n) ((uint64_t)(n) * 2000)
static const struct roger_key keys[] = {
    {99, MS(4000), MS(4300)},  {99, MS(10000), MS(10300)},
    {98, MS(16000), MS(17500)}, {56, MS(17500), MS(19000)},
    {67, MS(19000), MS(20500)}, {87, MS(20500), MS(22000)},
};

static const struct roger_config config = {
    .title = "Pac-Mania (recompiled)",
    .image_path = "../extracted/!PacMania/PACMANIA,fffff843-2bb2e27b",
    .image = &recomp_image,
    .programs = programs,
    .appdir = "../extracted/!PacMania",
    .dir_prefixes = prefixes,
    .command_line = "Pacmania",
    .initial_mode = 12,                 /* the desktop's; the program selects its own */
    .wimpslot = 620 * 1024,
    .keys = keys,
    .nkeys = sizeof keys / sizeof keys[0],
};

int main(int argc, char **argv) { roger_main(argc, argv, &config); }
