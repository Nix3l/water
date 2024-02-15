// CURRENT:
// TODO(nix3l): continue figuring out this memory model.
// more specifically how to do dynamic arrays
// TODO(nix3l): start with shaders (see shader.h)

#include "game.h"
#include "util/log.h"

// TODO(nix3l): this is a slightly older version of cimgui
// since i completely forget how i compiled the backends the first time around lol
// at some point probably try to compile the new backends. the new version of the main lib is still here though

#include "platform/platform.h"
#include "io/window.h"
#include "io/input.h"
#include "im_gui/im_gui.h"
#include "shader/forward_shader/forward_shader.h"

game_memory_s* game_memory = NULL;
game_state_s* game_state = NULL;

static void init_game_state(usize permenant_memory_to_allocate, usize transient_memory_to_allocate) {
    ASSERT(sizeof(game_state_s) < permenant_memory_to_allocate);

    // TODO(nix3l): check for allocation failure
    game_memory = mem_alloc(sizeof(game_memory_s));
    game_memory->permenant_storage_size = permenant_memory_to_allocate;
    game_memory->permenant_storage = mem_alloc(permenant_memory_to_allocate);
    MEM_ZERO(game_memory->permenant_storage, game_memory->permenant_storage_size);
    game_memory->transient_storage_size = transient_memory_to_allocate;
    game_memory->transient_storage = mem_alloc(transient_memory_to_allocate);
    MEM_ZERO(game_memory->transient_storage, game_memory->transient_storage_size);

    // PARTITIONING MEMORY
    game_state = game_memory->permenant_storage;
    MEM_ZERO_STRUCT(game_state);

    // NOTE(nix3l): for now just give the rest of the memory (that isnt used by game_state) for the next arena
    // later on we should partition the memory for each system accordingly
    game_state->shader_arena = arena_create_in_block(game_memory->permenant_storage + sizeof(game_state_s), permenant_memory_to_allocate - sizeof(game_state_s));

    // IO
    create_window(1280, 720, "test");
    init_input();

    // GUI
    init_imgui();

    init_forward_shader();

    float vertices[] = {
         0.5f,  0.5f, 0.0f, // top right
         0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f  // top left 
    };

    GLuint indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    game_state->test_mesh = create_mesh(vertices, NULL, NULL, NULL, indices, 6, 12);
}

static void terminate_game() {
    destroy_mesh(&game_state->test_mesh);
    
    shutdown_imgui();
    destroy_window();

    mem_free(game_memory->permenant_storage);
    mem_free(game_memory);
}

int main(void) {
    init_game_state(GIGABYTES(1), KILOBYTES(64));

    while(!glfwWindowShouldClose(game_state->window.glfw_window)) {
        render_mesh(&game_state->test_mesh);

        update_imgui();
        render_imgui();

        glfwSwapBuffers(game_state->window.glfw_window);

        update_input();
        glfwPollEvents();
    }

    terminate_game();

    return 0;
}
