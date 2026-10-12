# Static recompilation of Pac-Mania: the translated program and player (gen/,
# gen_player/, from superspider gen) and game.c, linked with libroger (the
# RISC OS runtime).
ROGER   ?= $(HOME)/stuff/libroger
SS      ?= $(HOME)/stuff/superspider
CC      ?= gcc
CFLAGS  ?= -O2 -g -w -fno-strict-aliasing $(if $(TRACE),-DRECOMP_TRACE)
GEN     := $(wildcard gen/code*.c) gen/entries.c $(wildcard gen_player/code*.c) gen_player/entries.c
OBJ     := $(GEN:.c=.o) game.o
LIBS    := $(shell pkg-config --libs sdl3) -lm -lpthread
INC     := -I$(ROGER)/include
APP     := ../game/!PacMania
PROG    := $(APP)/PACMANIA,fffff843-2bb2e27b
PLAYER  := $(APP)/PLAYERCODE,fffff843-21dc9687
SSBIN   := $(SS)/_build/default/bin/main.exe

pacmania: $(OBJ) $(ROGER)/libroger.a
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(ROGER)/libroger.a $(LIBS)

$(ROGER)/libroger.a: FORCE
	$(MAKE) -C $(ROGER) libroger.a

gen/%.o: gen/%.c $(ROGER)/include/roger_cpu.h gen/gen.h
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

gen_player/%.o: gen_player/%.c $(ROGER)/include/roger_cpu.h gen_player/gen.h
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

game.o: game.c $(ROGER)/include/roger.h
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

regen:
	cd ../analysis && SUPERSPIDER=$(SS) ./run.sh
	rm -rf gen gen_player
	$(SSBIN) gen '$(PROG)' 8000 ../analysis/pacmania.json --c gen
	$(SSBIN) gen '$(PLAYER)' 6f4b4 ../analysis/playercode.json --c gen_player \
	    --prefix P --tabprefix recomp_player --files 1 --covtag p

# "make roots" merges the gap entries a --trace-gaps run logged
# (untranslated.log, or LOG=...) into ../analysis/pacmania.roots and
# playercode.roots, so the next "make regen" translates them.
LOG ?= untranslated.log

roots:
	python3 $(SS)/tools/gap_roots.py '$(PROG)' 8000 ../analysis/pacmania.roots $(LOG)
	python3 $(SS)/tools/gap_roots.py '$(PLAYER)' 6f4b4 ../analysis/playercode.roots $(LOG)

clean:
	rm -f gen/*.o gen_player/*.o game.o pacmania

.PHONY: FORCE regen roots clean
