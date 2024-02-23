#include "forward_shader.h"

#include "game.h"
#include "util/log.h"

static void bind_attributes() {
    shader_s* shader = &game_state->forward_shader;
    shader_bind_attribute(shader, 0, "vs_position");
};

static void load_uniforms(void* data) {
    entity_s* entity = data;

    shader_uniform_locations_s* uniforms = &game_state->shader_uniforms;

    shader_load_float(uniforms->forward_time, game_state->curr_time);
    shader_load_mat4(uniforms->forward_transformation, entity_transformation_matrix(entity));
    shader_load_mat4(uniforms->forward_projection_view, camera_projection_view(&game_state->camera));
    shader_load_float(uniforms->forward_wavelength, game_state->wavelength);
    shader_load_float(uniforms->forward_amplitude, game_state->amplitude);
    shader_load_float(uniforms->forward_speed, game_state->speed);
    shader_load_float(uniforms->forward_wavelength_factor, game_state->wavelength_factor);
    shader_load_float(uniforms->forward_amplitude_factor, game_state->amplitude_factor);
    shader_load_int(uniforms->forward_num_waves, game_state->num_waves);
}

void init_forward_shader() {
    game_state->forward_shader = load_and_create_shader(
            "forward",
            "shader/forward_vs.glsl",
            "shader/forward_fs.glsl",
            bind_attributes,
            load_uniforms
        );

    shader_uniform_locations_s* uniforms = &game_state->shader_uniforms;

    uniforms->forward_time = shader_get_uniform(&game_state->forward_shader, "time");
    uniforms->forward_transformation = shader_get_uniform(&game_state->forward_shader, "transformation");
    uniforms->forward_projection_view = shader_get_uniform(&game_state->forward_shader, "projection_view");
    uniforms->forward_wavelength = shader_get_uniform(&game_state->forward_shader, "wavelength");
    uniforms->forward_amplitude = shader_get_uniform(&game_state->forward_shader, "amplitude");
    uniforms->forward_speed = shader_get_uniform(&game_state->forward_shader, "speed");
    uniforms->forward_wavelength_factor = shader_get_uniform(&game_state->forward_shader, "wavelength_factor");
    uniforms->forward_amplitude_factor = shader_get_uniform(&game_state->forward_shader, "amplitude_factor");
    uniforms->forward_num_waves = shader_get_uniform(&game_state->forward_shader, "num_waves");
}
