#include "forward_shader.h"

#include "game.h"
#include "util/log.h"

static void bind_attributes() {
    shader_s* shader = &game_state->forward_shader;
    shader_bind_attribute(shader, 0, "vs_position");
    shader_bind_attribute(shader, 1, "vs_uvs");
};

void init_forward_shader() {
    game_state->forward_shader = load_and_create_shader(
            "forward",
            "shader/forward_vs.glsl",
            "shader/forward_fs.glsl",
            bind_attributes
        );
}
