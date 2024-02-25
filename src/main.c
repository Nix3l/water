// CURRENT:
// TODO(nix3l): figure out how to load structs into shaders
// TODO(nix3l): finish the sum of sines implementation

#include "game.h"
#include "util/log.h"

// TODO(nix3l): this is a slightly older version of cimgui
// since i completely forget how i compiled the backends the first time around lol
// at some point probably try to compile the new backends. the new version of the main lib is still here though

#include "platform/platform.h"
#include "shader/forward_shader/forward_shader.h"

game_memory_s* game_memory = NULL;
game_state_s* game_state = NULL;

static void show_debug_stats_window() {
    if(is_key_pressed(GLFW_KEY_F1)) game_state->show_debug_stats_window = !game_state->show_debug_stats_window;
    if(!game_state->show_debug_stats_window) return;
    igBegin("stats", &game_state->show_debug_stats_window, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    // FRAME STATS
    igSeparator();
    igText("elapsed time: %lf\n", game_state->curr_time);
    igText("delta time: %f\n", game_state->delta_time);
    igText("frame count: %u\n", game_state->frame_count);
    igText("fps: %u\n", game_state->fps);

    igSeparator();

    // MEMORY
    igText("total transient memory: %u\n", game_memory->transient_storage_size);
    igText("total permenant memory: %u\n", game_memory->permenant_storage_size);

    igText("permenant memory in use: %u/%u\n",
            sizeof(game_state_s) + 
            game_state->shader_arena.size + 
            game_state->mesh_arena.size,
            game_memory->permenant_storage_size);
    igIndent(12.0f);
    igText("of which state: %u\n", sizeof(game_state_s));
    igText("of which shaders: %u/%u\n", game_state->shader_arena.size, game_state->shader_arena.capacity);
    igText("of which meshes: %u/%u\n", game_state->mesh_arena.size, game_state->mesh_arena.capacity);
    igUnindent(12.0f);

    igEnd();
}

static void show_settings_window() {
    if(is_key_pressed(GLFW_KEY_F2)) game_state->show_settings_window = !game_state->show_settings_window;
    if(!game_state->show_settings_window) return;

    igBegin("settings", &game_state->show_settings_window, ImGuiWindowFlags_None);

    // CAMERA
    igDragFloat("move speed", &game_state->camera.speed, 1.0f, 0.0f, MAX_f32, "%.2f", ImGuiSliderFlags_None);
    igDragFloat("sensetivity", &game_state->camera.sens, 10.0f, 0.0f, MAX_f32, "%.2f", ImGuiSliderFlags_None);

    igSeparator();

    // RENDERER
    if(igButton("render wireframe", (ImVec2) { .x = -1.0f, .y = 24.0f }))
        game_state->forward_renderer.render_wireframe = !game_state->forward_renderer.render_wireframe;

    igSeparator();

    // SHADER VARIABLES
    
    void* temp_mem = game_memory->transient_storage;
    MEM_ZERO(temp_mem, 8);
    if(igCollapsingHeader_TreeNodeFlags("waves", ImGuiTreeNodeFlags_None)) {
        igDragFloat("wavelength factor", &game_state->wavelength_factor, 0.01f, 0.0f, MAX_f32, "%.3f", ImGuiSliderFlags_None);
        igDragFloat("amplitude factor", &game_state->amplitude_factor, 0.01f, 0.0f, MAX_f32, "%.3f", ImGuiSliderFlags_None);

        for(u32 i = 0; i < TOTAL_WAVES; i ++) {
            wave_s wave = game_state->waves[i];
            char* label = temp_mem;
            snprintf(label, 8, "wave %u", i);
            
            if(!igTreeNode_Str(label)) continue;

            igDragFloat("wavelength", &wave.wavelength, 0.01f, 0.0f, MAX_f32, "%.3f", ImGuiSliderFlags_None);
            igDragFloat("amplitude", &wave.amplitude, 0.01f, 0.0f, MAX_f32, "%.3f", ImGuiSliderFlags_None);
            igDragFloat("speed", &wave.speed, 0.01f, 0.0f, MAX_f32, "%.3f", ImGuiSliderFlags_None);
            igDragFloat("angle", &wave.angle, 0.01f, 0.0f, 360.0f, "%.3f", ImGuiSliderFlags_None);
            igTreePop();
        }
    }

    igSeparator();

    igDragFloat3("light direction", game_state->sun.direction.raw, 0.1f, -1.0f, 1.0f, "%.3f", ImGuiSliderFlags_None);
    igColorEdit3("light color", game_state->sun.color.raw, ImGuiColorEditFlags_None);
    igDragFloat("light intensity", &game_state->sun.intensity, 0.01f, 0.0f, MAX_f32, "%.3f", ImGuiSliderFlags_None);
    igDragFloat("ambient", &game_state->ambient, 0.01f, 0.0f, MAX_f32, "%.3f", ImGuiSliderFlags_None);
    igColorEdit3("ambient color", game_state->ambient_color.raw, ImGuiColorEditFlags_None);

    igEnd();
}

// NOTE(nix3l): this is a very rudimentary version of time profiling
// later on i should implement a stack based thing where i can push and pop
// before/after actions to more accurately profile, but that goes beyond
// the scope of this project
static void update_frame_stats() {
    game_state->old_time = game_state->curr_time;
    game_state->curr_time = glfwGetTime();
    game_state->delta_time = game_state->curr_time - game_state->old_time;
    game_state->frame_count ++;
    game_state->fps_counter ++;
    game_state->fps_timer += game_state->delta_time;

    // update fps every 0.16s or so
    // makes it flicker less than updating it every frame
    if(game_state->fps_timer >= (1/6.0f)) {
        game_state->fps = game_state->fps_counter / game_state->fps_timer;
        game_state->fps_counter = 0;
        game_state->fps_timer = 0.0f;
    }
}

static arena_s partition_permenant_memory(void** mem, usize size, usize* remaining) {
    ASSERT(size <= *remaining);
    arena_s arena = arena_create_in_block(*mem, size);

    *remaining -= size;
    *mem += size;
    return arena;
}

static void init_game_state(usize permenant_memory_to_allocate, usize transient_memory_to_allocate) {
    ASSERT(sizeof(game_state_s) < permenant_memory_to_allocate);

    // INITIALISE MEMORY
    game_memory = mem_alloc(sizeof(game_memory_s));
    ASSERT(game_memory);

    game_memory->permenant_storage_size = permenant_memory_to_allocate;
    game_memory->permenant_storage = mem_alloc(permenant_memory_to_allocate);
    ASSERT(game_memory->permenant_storage);
    MEM_ZERO(game_memory->permenant_storage, game_memory->permenant_storage_size);
    
    game_memory->transient_storage_size = transient_memory_to_allocate;
    game_memory->transient_storage = mem_alloc(transient_memory_to_allocate);
    ASSERT(game_memory->transient_storage);
    MEM_ZERO(game_memory->transient_storage, game_memory->transient_storage_size);

    // PARTITIONING MEMORY
    game_state = game_memory->permenant_storage;
    MEM_ZERO_STRUCT(game_state);

    void* memory = game_memory->permenant_storage + sizeof(game_state_s);
    usize remaining_memory = permenant_memory_to_allocate - sizeof(game_state_s);
    
    game_state->shader_arena = partition_permenant_memory(&memory, KILOBYTES(4), &remaining_memory);
    game_state->mesh_arena = partition_permenant_memory(&memory, remaining_memory, &remaining_memory);

    // IO
    create_window(1280, 720, "water");
    init_input();

    // SHADERS
    init_forward_shader();

    for(int i = 0; i < TOTAL_WAVES; i ++) {
        game_state->waves[i].wavelength = 3.0f;
        game_state->waves[i].amplitude = 0.30f;
        game_state->waves[i].speed = 3.0f;
        game_state->waves[i].angle = 0.0f;
    }

    game_state->wavelength_factor = 1.18f;
    game_state->amplitude_factor = 0.78f;

    // RENDERER
    game_state->camera = (camera_s) {
        .position   = VECTOR_3(0.0f, 12.0f, 0.0f),
        .rotation   = VECTOR_3_ZERO(),
        
        .fov        = 70.0f,
        .near_plane = 0.001f,
        .far_plane  = 1000.0f,

        .speed      = 12.0f,
        .sens       = 7500.0f
    };

    game_state->sun = (directional_light_s) {
        .color = VECTOR_3(1.0f, 1.0f, 1.0f),
        .direction = VECTOR_3(-0.1f, -0.3f, 0.04f),
        .intensity = 0.5f
    };

    game_state->ambient = 0.1f;
    game_state->ambient_color = VECTOR_3(191.0f/255.0f, 181.0f/255.0f, 121.0f/255.0f);

    init_forward_renderer();

    // GUI
    init_imgui();

    game_state->test_entity.mesh = primitive_plane_mesh(
        VECTOR_3(-128.0f, 0.0f, -128.0f),
        (v2i) { .x = 512, .y = 512 },
        VECTOR_2(256.0f, 256.0f),
        &game_state->mesh_arena
    );

    game_state->test_entity.transform = (transform_s) {
        .position = VECTOR_3_ZERO(),
        .rotation = VECTOR_3_ZERO(),
        .scale    = VECTOR_3_ONE()
    };

    glfwSetInputMode(game_state->window.glfw_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

static void terminate_game() {
    destroy_mesh(&game_state->test_entity.mesh);

    destroy_shader(&game_state->forward_shader);
    
    shutdown_imgui();
    destroy_window();

    mem_free(game_memory->transient_storage);
    mem_free(game_memory->permenant_storage);
    mem_free(game_memory);
}

int main(void) {
    init_game_state(GIGABYTES(1), MEGABYTES(16));

    while(!glfwWindowShouldClose(game_state->window.glfw_window)) {
        update_frame_stats();

        update_camera(&game_state->camera);
        render_forward(&game_state->test_entity);

        update_imgui();
        show_debug_stats_window();
        show_settings_window();
        render_imgui();

        glfwSwapBuffers(game_state->window.glfw_window);

        update_input();
        glfwPollEvents();
    }

    terminate_game();

    return 0;
}
