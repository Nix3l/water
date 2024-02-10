#include "game.h"
#include "util/log.h"

// TODO(nix3l): this is a slightly older version of cimgui
// since i completely forget how i compiled the backends the first time around lol
// at some point probably try to compile the new backends. the new version of the main lib is still here though

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui.h"

#define CIMGUI_USE_GLFW
#define CIMGUI_USE_OPENGL3
#include "cimgui_impl.h"

#include "platform/platform.h"
#include "io/window.h"
game_state_s* game_state = NULL;

// TODO(nix3l): move the imgui functions to a separate file

static struct ImGuiContext* imgui_ctx;
static struct ImGuiIO* imgui_io;

static void init_imgui() {
    imgui_ctx = igCreateContext(NULL);
    imgui_io = igGetIO();

    const char* glsl_ver = "#version 330 core";
    ImGui_ImplGlfw_InitForOpenGL(game_state->window->glfw_window, true);
    ImGui_ImplOpenGL3_Init(glsl_ver);

    igStyleColorsDark(NULL);
}

static void shutdown_imgui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    igDestroyContext(imgui_ctx);
}

static void render_imgui() {
    igRender();
    ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
}

static void update_imgui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    igNewFrame();

    igShowDemoWindow(NULL);
}

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
