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

mesh_s create_mesh(float* vertex_data, int num_vertices, GLuint* indices, int num_indices) {
    mesh_s mesh;
    mesh.data = MESH_VERTICES;
    mesh.vertices = vertex_data;
    mesh.indices = indices;

    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);

    mesh.vertices_vbo = create_vbo(0, 3, vertex_data, num_vertices);
    mesh.indices_vbo = create_indices_vbo(indices, num_indices);
    mesh.index_count = num_indices;

    glBindVertexArray(0);

    return mesh;
}

void render_mesh(mesh_s* mesh) {
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0, 0, game_state->window.width, game_state->window.height);

    glBindVertexArray(mesh->vao);
    glEnableVertexAttribArray(0);

    glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}

void destroy_mesh(mesh_s* mesh) {
    // TODO(nix3l): free mesh memory
    glDeleteBuffers(1, &mesh->vertices_vbo);
    glDeleteBuffers(1, &mesh->uvs_vbo);
    glDeleteBuffers(1, &mesh->normals_vbo);
    glDeleteBuffers(1, &mesh->colors_vbo);
    glDeleteBuffers(1, &mesh->indices_vbo);

    glDeleteVertexArrays(1, &mesh->vao);
}
