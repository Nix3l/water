#include "renderer.h"

#include "game.h"

void init_forward_renderer() {
    game_state->forward_renderer = (forward_renderer_s) {
        .background_color = VECTOR_4(1.0f, 1.0f, 1.0f, 1.0f),
        .shader = &game_state->forward_shader,
        .render_wireframe = false,
        .framebuffer = create_fbo(game_state->window.width, game_state->window.height, 2)
    };

    // no transparency for now
    fbo_create_texture(&game_state->forward_renderer.framebuffer,
            GL_COLOR_ATTACHMENT0, 
            GL_RGB16F,
            GL_RGB);

    fbo_create_texture(&game_state->forward_renderer.framebuffer,
            GL_DEPTH_ATTACHMENT,
            GL_DEPTH_COMPONENT32F,
            GL_DEPTH_COMPONENT);
}

void render_forward(entity_s* entity) {
    forward_renderer_s* renderer = &game_state->forward_renderer;
    fbo_s* framebuffer = &renderer->framebuffer;
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->id);

    glDrawBuffers(framebuffer->num_textures - 1, framebuffer->attachments);

    v4f bg = game_state->forward_renderer.background_color;
    glClearColor(bg.x, bg.y, bg.z, bg.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glViewport(0, 0, game_state->window.width, game_state->window.height);

    shader_start(&game_state->forward_shader);

    game_state->forward_shader.load_uniforms(entity);

    if(game_state->forward_renderer.render_wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glBindVertexArray(entity->mesh.vao);
    mesh_enable_attributes(&entity->mesh);

    glDrawElements(GL_TRIANGLES, entity->mesh.index_count, GL_UNSIGNED_INT, 0);

    mesh_disable_attributes(&entity->mesh);
    glBindVertexArray(0);

    if(game_state->forward_renderer.render_wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    shader_stop();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
