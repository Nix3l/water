#include "renderer.h"

#include "game.h"

void init_forward_renderer() {
    game_state->forward_renderer = (forward_renderer_s) {
        .background_color = VECTOR_4(1.0f, 1.0f, 1.0f, 1.0f),
        .shader = &game_state->forward_shader
    };
}

void render_forward(entity_s* entity) {
    v4f bg = game_state->forward_renderer.background_color;
    glClearColor(bg.x, bg.y, bg.z, bg.w);
    glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0, 0, game_state->window.width, game_state->window.height);

    shader_start(&game_state->forward_shader);

    game_state->forward_shader.load_uniforms(entity);

    glBindVertexArray(entity->mesh.vao);
    mesh_enable_attributes(&entity->mesh);

    glDrawElements(GL_TRIANGLES, entity->mesh.index_count, GL_UNSIGNED_INT, 0);

    mesh_disable_attributes(&entity->mesh);
    glBindVertexArray(0);

    shader_stop();
}
