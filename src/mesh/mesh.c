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

void destroy_mesh(mesh_s* mesh) {
    glDeleteBuffers(1, &mesh->vertices_vbo);
    glDeleteBuffers(1, &mesh->uvs_vbo);
    glDeleteBuffers(1, &mesh->normals_vbo);
    glDeleteBuffers(1, &mesh->colors_vbo);
    glDeleteBuffers(1, &mesh->indices_vbo);

    glDeleteVertexArrays(1, &mesh->vao);
}

mesh_s primitive_plane_mesh(v3f bottom_left, v2i num_vertices, v2f world_size) {
    // TODO(nix3l): uvs + normals
    // NOTE(nix3l): colors are not accounted for in this function
    // however i can always create another one or pass in a color for all the vertices
    // as a parameter to this function if need be
    u32 total_vertices = num_vertices.x * num_vertices.y;

    // -1 to account for the shared vertices
    // i.e. the edge vertices only belong to one face, whereas
    // all the other vertices belong to 2-4
    u32 total_faces = (num_vertices.x - 1) * (num_vertices.y - 1);

    // 2 traingles per quad, 3 indices per triangle
    // therefore 6 indices per quad
    u32 total_indices = total_faces * 6;
    
    arena_s temp_mem = arena_create_in_block(game_memory->transient_storage, game_memory->transient_storage_size);
    v3f* vertices = arena_push(&temp_mem, sizeof(v3f) * total_vertices);
    GLuint* indices = arena_push(&temp_mem, sizeof(GLuint) * total_indices);

    f32 x_step = world_size.x / (num_vertices.x - 1);
    f32 z_step = world_size.y / (num_vertices.y - 1);

    for(u32 y = 0; y < num_vertices.y; y ++) {
        for(u32 x = 0; x < num_vertices.x; x ++) {
            vertices[x + (y * num_vertices.x)].x = bottom_left.x + (x_step * x);
            vertices[x + (y * num_vertices.x)].y = bottom_left.y;
            vertices[x + (y * num_vertices.x)].z = bottom_left.z + (z_step * y);
        }
    }

    // this offset is kind of hard to explain 
    // but essentially whenever we hit an edge, the vertex at the very edge
    // will belong to less faces than faces in the middle
    // essentially this offset lets us account for that
    // as we are iterating through the vertices
    // from bottom left to top right, row by row
    u32 offset = 0;
    for(u32 i = 0; i < total_indices; i ++) {
        // bottom left vertex in the current face
        u32 face_bottom_left = i / 6 + offset;

        // if we are on an edge
        if((face_bottom_left + 1) % num_vertices.x == 0) {
            offset ++;
            face_bottom_left ++;
        }

        // keep in mind that we are going from bottom left to top right 

        // first triangle
        indices[i++] = face_bottom_left; // bottom left
        indices[i++] = face_bottom_left + num_vertices.x; // top left
        indices[i++] = face_bottom_left + num_vertices.x + 1; // top right

        // second triangle
        indices[i++] = face_bottom_left; // bottom left
        indices[i++] = face_bottom_left + num_vertices.x + 1; // top right
        indices[i]   = face_bottom_left + 1; // bottom right
    }

    f32* vertices_raw = arena_push(&temp_mem, sizeof(f32) * total_vertices * 3);
    f32* curr_vertex_value = vertices_raw;
    for(u32 i = 0; i < total_vertices; i ++) {
        *(curr_vertex_value++) = vertices[i].x;
        *(curr_vertex_value++) = vertices[i].y;
        *(curr_vertex_value++) = vertices[i].z;

        LOG("vertex [%.3f, %.3f, %.3f]\n", vertices[i].x, vertices[i].y, vertices[i].z);
    }

    // TODO(nix3l): figure out storing this data in some arena somewhere
    // technically we are currently feeding it trash data once we are done here
    mesh_s mesh = create_mesh(
            vertices_raw,
            NULL, NULL, NULL,
            indices,
            total_indices,
            total_vertices
        );

    MEM_ZERO(game_memory->transient_storage, game_memory->transient_storage_size);

    return mesh;
}
