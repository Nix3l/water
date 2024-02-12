#include "im_gui.h"

#include "game.h"

static struct ImGuiContext* imgui_ctx;
static struct ImGuiIO* imgui_io;

void init_imgui() {
    imgui_ctx = igCreateContext(NULL);
    imgui_io = igGetIO();

    const char* glsl_ver = "#version 330 core";
    ImGui_ImplGlfw_InitForOpenGL(game_state->window.glfw_window, true);
    ImGui_ImplOpenGL3_Init(glsl_ver);

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
