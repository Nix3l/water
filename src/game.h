#ifndef GAME_H
#define GAME_H

#include "base.h"

#include "memory/memory.h"
#include "io/window.h"

// TODO(nix3l): figure out whether keeping some variables global
// like the window here is better than keeping them in the .c files as static
// maybe move the memory to a struct and keep the state vars in this one? not sure
// regardless having both the memory and the pointers to said memory
// in the same struct does not sit with me well.
typedef struct {
    arena_s game_memory;

    window_s* window;
} game_state_s;

extern game_state_s* game_state;

#endif
