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
#include "camera/camera.h"

typedef struct {
    usize permenant_storage_size;
    void* permenant_storage; // must be cleared to zero on startup

    usize transient_storage_size;
    void* transient_storage; // must be cleared to zero on startup
} game_memory_s;

typedef struct {
    // IO
    window_s window;
    input_state_s input_state;

    // FRAME STATS
    f64 old_time;
    f64 curr_time;
    f32 delta_time;
    f32 fps_timer;
    u32 frame_count;
    u32 fps;

    // SHADERS
    arena_s shader_arena; // mostly contains text for shader paths/names
    shader_uniform_locations_s shader_uniforms;
    shader_s forward_shader;

    // RENDERER
    camera_s camera;
    forward_renderer_s forward_renderer;

    // MESH
    arena_s mesh_arena; // contains the raw data of the meshes

    // IMGUI
    struct ImGuiContext* imgui_ctx;
    struct ImGuiIO* imgui_io;

    // OTHER
    entity_s test_entity;

    bool show_debug_stats_window;
    bool show_settings_window;
} game_state_s;

extern game_memory_s* game_memory;
extern game_state_s* game_state;

// not a big fan of having this macro in this header but i do not care honestly
// much more important things to take care of than proper code placement
// especially when its this insignificant
#define delta_time() (game_state->delta_time)

#endif
