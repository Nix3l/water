#ifndef MESH_H
#define MESH_H

#include "base.h"

typedef enum {
    MESH_VERTICES = 0x01,
    MESH_UVS = 0x02,
    MESH_NORMALS = 0x04,
    MESH_COLORS = 0x08,
} mesh_data_flags_t;

enum {
    MESH_ATTRIBUTE_VERTICES = 0,
    MESH_ATTRIBUTE_UVS = 1,
    MESH_ATTRIBUTE_NORMALS = 2,
    MESH_ATTRIBUTE_COLORS = 3,
};

typedef struct {
    // TODO(nix3l): arena for mesh memory
    // arena_s memory;

    // metadata
    char* name;
    char* full_path;

    // data
    mesh_data_flags_t data;

    float* vertices;
    float* uvs;
    float* normals;
    float* colors;

    GLuint* indices;

    u32 index_count; // only for indices
    u32 vertex_count; // same for all vertex data

    // opengl data
    GLuint vao;

    GLuint vertices_vbo;
    GLuint uvs_vbo;
    GLuint normals_vbo;
    GLuint colors_vbo;

    GLuint indices_vbo;
} mesh_s;

mesh_s create_mesh(
        float* vertex_data, float* uvs_data, float* normals_data, float* colors_data, 
        GLuint* indices,
        int num_indices, int num_vertices);

void mesh_enable_attributes(mesh_s* mesh);
void mesh_disable_attributes(mesh_s* mesh);

void destroy_mesh(mesh_s* mesh);
void render_mesh(mesh_s* mesh);

#endif