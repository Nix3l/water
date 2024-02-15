#include "mesh.h"
#include "game.h"
#include "util/log.h"

static GLuint create_vbo(GLuint attribute, GLuint dimensions, float* data, int data_size) {
    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, data_size * sizeof(GLfloat), data, GL_STATIC_DRAW);
    glVertexAttribPointer(attribute, dimensions, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return vbo;
}

static GLuint create_indices_vbo(GLuint* indices, int num_indices) {
    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(GLuint), indices, GL_STATIC_DRAW);

    // TODO(nix3l): unbind?
    return vbo;
}

mesh_s create_mesh(
        float* vertex_data, float* uvs_data, float* normals_data, float* colors_data, 
        GLuint* indices,
        int num_indices, int num_vertices) {
    mesh_s mesh;

    // TODO(nix3l): name and full path

    if(!vertex_data || num_vertices == 0) {
        LOG_ERR("tried to load mesh without vertex data!\n");
        return (mesh_s) {};
    }

    mesh.vertices = vertex_data;
    mesh.uvs = uvs_data;
    mesh.normals = normals_data;
    mesh.colors = colors_data;
    mesh.indices = indices;

    mesh.data = MESH_VERTICES;
    if(uvs_data) mesh.data |= MESH_UVS;
    if(normals_data) mesh.data |= MESH_NORMALS;
    if(colors_data) mesh.data |= MESH_COLORS;

    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);

    mesh.vertices_vbo = create_vbo(MESH_ATTRIBUTE_VERTICES, 3, vertex_data, num_vertices);
    mesh.uvs_vbo = uvs_data ? create_vbo(MESH_ATTRIBUTE_UVS, 2, uvs_data, num_vertices) : 0;
    mesh.normals_vbo = normals_data ? create_vbo(MESH_ATTRIBUTE_NORMALS, 3, normals_data, num_vertices) : 0;
    mesh.colors_vbo = colors_data ? create_vbo(MESH_ATTRIBUTE_COLORS, 3, colors_data, num_vertices) : 0;
    mesh.indices_vbo = create_indices_vbo(indices, num_indices);

    mesh.index_count = num_indices;
    mesh.vertex_count = num_vertices;

    glBindVertexArray(0);

    return mesh;
}

void mesh_enable_attributes(mesh_s* mesh) {
    if(mesh->data & MESH_VERTICES)
        glEnableVertexAttribArray(MESH_ATTRIBUTE_VERTICES);
    if(mesh->data & MESH_UVS)
        glEnableVertexAttribArray(MESH_ATTRIBUTE_UVS);
    if(mesh->data & MESH_NORMALS)
        glEnableVertexAttribArray(MESH_ATTRIBUTE_NORMALS);
    if(mesh->data & MESH_COLORS)
        glEnableVertexAttribArray(MESH_ATTRIBUTE_COLORS);
}

void mesh_disable_attributes(mesh_s* mesh) {
    if(mesh->data & MESH_VERTICES)
        glDisableVertexAttribArray(MESH_ATTRIBUTE_VERTICES);
    if(mesh->data & MESH_UVS)
        glDisableVertexAttribArray(MESH_ATTRIBUTE_UVS);
    if(mesh->data & MESH_NORMALS)
        glDisableVertexAttribArray(MESH_ATTRIBUTE_NORMALS);
    if(mesh->data & MESH_COLORS)
        glDisableVertexAttribArray(MESH_ATTRIBUTE_COLORS);
}

void render_mesh(mesh_s* mesh) {
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0, 0, game_state->window.width, game_state->window.height);

    shader_start(&game_state->forward_shader);

    game_state->forward_shader.load_uniforms(NULL);

    glBindVertexArray(mesh->vao);
    mesh_enable_attributes(mesh);

    glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0);

    mesh_disable_attributes(mesh);
    glBindVertexArray(0);

    shader_stop();
}

void destroy_mesh(mesh_s* mesh) {
    glDeleteBuffers(1, &mesh->vertices_vbo);
    glDeleteBuffers(1, &mesh->uvs_vbo);
    glDeleteBuffers(1, &mesh->normals_vbo);
    glDeleteBuffers(1, &mesh->colors_vbo);
    glDeleteBuffers(1, &mesh->indices_vbo);

    glDeleteVertexArrays(1, &mesh->vao);
}
