#ifndef SHADER_H
#define SHADER_H

#include "base.h"

typedef struct {
    // metadata
    char* name;
    char* vertex_full_path;
    char* fragment_full_path;

    GLuint program_id;

    // NOTE(nix3l): there might be no real reason to hold on to these after creation
    // see create_shader in shader.c for more info
    GLuint vertex_id;
    GLuint fragment_id;

    // TODO(nix3l): UNIFORMS
} shader_s;

shader_s create_shader(char* name, char* vertex_src, char* fragment_src, void (*bind_attributes) ());
shader_s load_and_create_shader(char* name, char* vertex_path, char* fragment_path, void (*bind_attributes) ());
void destroy_shader(shader_s* shader);

void shader_start(shader_s* shader);
void shader_stop();

GLuint load_uniform(shader_s* shader, char* uniform_name);

#endif
