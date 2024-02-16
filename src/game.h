#ifndef GAME_H
#define GAME_H

#include "base.h"

#include "im_gui/im_gui.h"

#include "memory/memory.h"
#include "io/window.h"
#include "io/input.h"
#include "mesh/mesh.h"
#include "shader/shader.h"
#include "entity/entity.h"
#include "render/renderer.h"

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

    // SHADERS
    arena_s shader_arena; // mostly contains text for shader paths/names
    shader_uniform_locations_s shader_uniforms;
    shader_s forward_shader;

    // RENDERER
    forward_renderer_s forward_renderer;

    // IMGUI
    struct ImGuiContext* imgui_ctx;
    struct ImGuiIO* imgui_io;

    // OTHER
    entity_s test_entity;
} game_state_s;

extern game_memory_s* game_memory;
extern game_state_s* game_state;

#endif
