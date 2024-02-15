#ifndef GAME_H
#define GAME_H

#include "base.h"

#include "memory/memory.h"
#include "io/window.h"
#include "io/input.h"
#include "mesh/mesh.h"
#include "shader/shader.h"

typedef struct {
    usize permenant_storage_size;
    void* permenant_storage; // must be cleared to zero on startup

    usize transient_storage_size;
    void* transient_storage; // must be cleared to zero on startup

    // NOTE(nix3l): put transient storage in here when necessary
} game_memory_s;

typedef struct {
    // IO
    window_s window;
    input_state_s input_state;

    arena_s shader_arena;
    shader_s forward_shader;

    mesh_s test_mesh;
} game_state_s;

extern game_memory_s* game_memory;
extern game_state_s* game_state;

#endif
