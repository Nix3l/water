#include "forward_shader.h"

#include "game.h"
#include "util/log.h"

static void bind_attributes() {
    shader_s* shader = &game_state->forward_shader;
    shader_bind_attribute(shader, 0, "vs_position");
};

static void load_uniforms(void* data) {
    entity_s* entity = data;
    shader_load_mat4(game_state->shader_uniforms.forward_transformation, entity_transformation_matrix(entity));
}

void init_forward_shader() {
    game_state->forward_shader = load_and_create_shader(
            "forward",
            "shader/forward_vs.glsl",
            "shader/forward_fs.glsl",
            bind_attributes,
            load_uniforms
        );

    game_state->shader_uniforms.forward_transformation = shader_get_uniform_location(&game_state->forward_shader, "transformation");
}
