#ifndef WINDOW_H
#define WINDOW_H

#include "base.h"
#include "memory/memory.h"

typedef struct {
    GLFWwindow* glfw_window;

    char* title;
    u16 width;
    u16 height;
} window_s;

void create_window(u16 width, u16 height, char* title);
void destroy_window();

#endif
