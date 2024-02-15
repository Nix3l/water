#include "forward_shader.h"

#include "game.h"

static void bind_attributes() {
    
};

void init_forward_shader() {
    game_state->forward_shader = create_shader("forward", "", "", bind_attributes);
}
