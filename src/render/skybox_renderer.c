#include "skybox_renderer.h"

#include "game.h"

void init_skybox_renderer() {
    float vertices[36 * 3] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    game_state->skybox_renderer = (skybox_renderer_s) {
        .mesh = create_mesh_arrays(vertices, NULL, NULL, NULL, 36)
    };
}

void render_skybox(texture_s* cubemap, fbo_s* framebuffer) {
    if(!cubemap) return;

    skybox_renderer_s* renderer = &game_state->skybox_renderer;

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->id);
    glDrawBuffers(framebuffer->num_textures, framebuffer->attachments);

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glViewport(0, 0, game_state->window.width, game_state->window.height);

    shader_start(&game_state->skybox_shader);

    game_state->skybox_shader.load_uniforms(NULL);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap->id);

    glBindVertexArray(renderer->mesh.vao);
    mesh_enable_attributes(&renderer->mesh);

    glDrawArrays(GL_TRIANGLES, 0, renderer->mesh.vertex_count);

    mesh_disable_attributes(&renderer->mesh);
    glBindVertexArray(0);

    shader_stop();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
