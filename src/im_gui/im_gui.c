#include "im_gui.h"

#include "game.h"

// TODO(nix3l): move these to the game_state struct
static struct ImGuiContext* imgui_ctx;
static struct ImGuiIO* imgui_io;

void init_imgui() {
    imgui_ctx = igCreateContext(NULL);
    imgui_io = igGetIO();

    ImGui_ImplGlfw_InitForOpenGL(game_state->window.glfw_window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    igStyleColorsDark(NULL);
}

void shutdown_imgui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    igDestroyContext(imgui_ctx);
}

void update_imgui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    igNewFrame();
}

void render_imgui() {
    igRender();
    ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
}
