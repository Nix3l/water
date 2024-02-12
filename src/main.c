// CURRENT:
// TODO(nix3l): start with the basic renderer

#include "game.h"
#include "util/log.h"

// TODO(nix3l): this is a slightly older version of cimgui
// since i completely forget how i compiled the backends the first time around lol
// at some point probably try to compile the new backends. the new version of the main lib is still here though

#include "platform/platform.h"
#include "io/window.h"
#include "io/input.h"
#include "im_gui/im_gui.h"

game_state_s* game_state = NULL;

void init_game_state(u64 memory_to_allocate) {
    ASSERT(sizeof(game_state_s) < memory_to_allocate);

    game_state = mem_alloc(memory_to_allocate);
    MEM_ZERO_STRUCT(game_state);

    // IO
    create_window(1280, 720, "test");
    init_input();

    // GUI
    init_imgui();
}

void terminate_game() {
    shutdown_imgui();
    destroy_window();

    mem_free(game_state);
}

int main(void) {
    init_game_state(GIGABYTES(1));

    while(!glfwWindowShouldClose(game_state->window.glfw_window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        update_imgui();
        render_imgui();

        glfwSwapBuffers(game_state->window.glfw_window);

        update_input();
        glfwPollEvents();
    }

    terminate_game();

    return 0;
}
