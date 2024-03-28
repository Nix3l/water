#include "post_processing_shader.h"

#include "game.h"

static void bind_attributes() {
    shader_s* shader = &game_state->post_processing_shader;
    shader_bind_attribute(shader, 0, "vs_position");
    shader_bind_attribute(shader, 1, "vs_uvs");
}

static void load_uniforms(void* data) {
    shader_s* shader = &game_state->post_processing_shader;
    pproc_shader_uniforms_s* uniforms = &game_state->post_processing_shader_uniforms;

    shader_load_int(uniforms->scene, 0);
    shader_load_int(uniforms->depth, 1);

    shader_load_float(uniforms->near_plane, game_state->camera.near_plane);
    shader_load_float(uniforms->far_plane, game_state->camera.far_plane);
}

void init_post_processing_shaders() {
    game_state->post_processing_shader = load_and_create_shader(
            "postprocessing",
            "shader/post_process_vs.glsl",
            "shader/post_process_fs.glsl",
            bind_attributes,
            load_uniforms
        );

    shader_s* shader = &game_state->post_processing_shader;
    pproc_shader_uniforms_s* uniforms = &game_state->post_processing_shader_uniforms;

    uniforms->scene      = shader_get_uniform(shader, "scene_tex");
    uniforms->depth      = shader_get_uniform(shader, "depth_tex");
    uniforms->near_plane = shader_get_uniform(shader, "near_plane");
    uniforms->far_plane  = shader_get_uniform(shader, "far_plane");
}
