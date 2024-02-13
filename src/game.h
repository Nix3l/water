#ifndef GAME_H
#define GAME_H

#include "base.h"

#include "memory/memory.h"
#include "io/window.h"
#include "io/input.h"
#include "mesh/mesh.h"

typedef struct {
    // IO
    window_s window;
    input_state_s input_state;

    mesh_s test_mesh;
} game_state_s;

extern game_state_s* game_state;

#endif
