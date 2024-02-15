#include "forward_shader.h"

#include "game.h"
#include "util/log.h"

static void bind_attributes() {
    LOG("binding the attributes for forward shader\n");
};

void init_forward_shader() {
    game_state->forward_shader = load_and_create_shader(
            "forward",
            "shader/forward_vs.glsl",
            "shader/forward_fs.glsl",
            bind_attributes
        );
}
