#include "window.h"
#include "util/log.h"
#include "game.h"

static void glfw_error_callback(int error, const char* text) {
    LOG_ERR("GLFW ERROR [%d]: `%s`\n", error, text);
}

void create_window(u16 width, u16 height, char* title) {
    ASSERT(glfwInit());

    // hint to opengl v3.3 core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    game_state->window.width = width;
    game_state->window.height = height;
    game_state->window.title = title;

    game_state->window.glfw_window = glfwCreateWindow(width, height, title, NULL, NULL);
    if(!game_state->window.glfw_window) {
        glfwTerminate();
        LOG_ERR("could not create window!\n");
        ASSERT_BREAK(!window->window);
    }

    glfwMakeContextCurrent(game_state->window.glfw_window);

    // load all the opengl functions from the drivers with glad
    ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));

    glfwSetErrorCallback(glfw_error_callback);
}

void destroy_window() {
    glfwTerminate();
    glfwDestroyWindow(game_state->window.glfw_window);
}
