#ifndef GAME_H
#define GAME_H

#include "base.h"

#include "im_gui/im_gui.h"

#include "memory/memory.h"
#include "io/window.h"
#include "io/input.h"
#include "mesh/mesh.h"
#include "shader/shader.h"
#include "shader/forward_shader/forward_shader.h"
#include "entity/entity.h"
#include "render/renderer.h"
#include "camera/camera.h"
#include "light/light.h"

typedef struct {
    usize permenant_storage_size;
    void* permenant_storage; // must be cleared to zero on startup

    usize transient_storage_size;
    void* transient_storage; // must be cleared to zero on startup
} game_memory_s;

typedef struct {
    // ARENAS
    arena_s shader_arena; // mostly contains text for shader paths/names
    arena_s mesh_arena; // contains text for mesh names/filepaths

    // IO
    window_s window;
    input_state_s input_state;

    // FRAME STATS
    f64 old_time;
    f64 curr_time;
    f32 delta_time;
    u32 frame_count;
    f32 fps_timer;
    u32 fps_counter;
    u32 fps;

    // SHADERS
    forward_shader_uniforms_s forward_shader_uniforms;
    shader_s forward_shader;
    
    // WATER SHADER VARIABLES
    wave_s waves[TOTAL_WAVES];
    f32 wavelength_factor;
    f32 amplitude_factor;

    v2f wavelength_range;
    v2f amplitude_range;
    v2f steepness_range;
    v2f speed_range;
    v2f direction_range;

    v3f water_color;

    v3f tip_color;
    f32 tip_attenuation;

    f32 specular_factor;
    f32 specular_strength;

    f32 refractive_index;

    f32 ambient;
    v3f ambient_color;

    // RENDERER
    camera_s camera;
    directional_light_s sun;
    forward_renderer_s forward_renderer;

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
