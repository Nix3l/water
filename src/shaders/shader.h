#ifndef SHADER_H
#define SHADER_H

#include "base.h"

typedef struct {
    // metadata
    char* name;
    char* vertex_full_path;
    char* fragment_full_path;

    GLuint program_id;

    GLuint vertex_id;
    GLuint fragment_id;

    // TODO UNIFORMS
} shader_s;

shader_s create_shader();
void compile_shader(shader_s* shader);

void destroy_shader(shader_s* shader);

void shader_start(shader_s* shader);
void shader_stop(shader_s* shader);

#endif
