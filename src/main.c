#include "game.h"
#include "util/log.h"

// TODO(nix3l): this is a slightly older version of cimgui
// since i completely forget how i compiled the backends the first time around lol
// at some point probably try to compile the new backends. the new version of the main lib is still here though

#include "platform/platform.h"
#include "io/window.h"
#include "im_gui/im_gui.h"

game_state_s* game_state = NULL;

void init_game_state(u64 memory_to_allocate) {
    game_state = mem_alloc(sizeof(game_state_s));
    MEM_ZERO_STRUCT(game_state);
    game_state->game_memory = arena_create(memory_to_allocate);

    create_window(1280, 720, "test", &game_state->game_memory);
    init_imgui();
}

void terminate_game() {
    shutdown_imgui();
    destroy_window();

    arena_free(&game_state->game_memory);
    mem_free(game_state);
}

int main(void) {
    init_game_state(GIGABYTES(1));

    while(!glfwWindowShouldClose(game_state->window->glfw_window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        update_imgui();
        render_imgui();

        glfwSwapBuffers(game_state->window->glfw_window);
        glfwPollEvents();
    }

    terminate_game();

    return 0;
}
