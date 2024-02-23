#include "forward_shader.h"

#include "game.h"
#include "util/log.h"

static void bind_attributes() {
    shader_s* shader = &game_state->forward_shader;
    shader_bind_attribute(shader, 0, "vs_position");
    shader_bind_attribute(shader, 1, "vs_uvs");
    shader_bind_attribute(shader, 2, "vs_normals");
};

static void load_uniforms(void* data) {
    entity_s* entity = data;

    shader_uniform_locations_s* uniforms = &game_state->shader_uniforms;

    // VERTEX SHADER
    shader_load_float(uniforms->forward_time, game_state->curr_time);
    shader_load_mat4(uniforms->forward_transformation, entity_transformation_matrix(entity));
    shader_load_mat4(uniforms->forward_projection_view, camera_projection_view(&game_state->camera));
    shader_load_float(uniforms->forward_wavelength, game_state->wavelength);
    shader_load_float(uniforms->forward_amplitude, game_state->amplitude);
    shader_load_float(uniforms->forward_speed, game_state->speed);
    shader_load_float(uniforms->forward_wavelength_factor, game_state->wavelength_factor);
    shader_load_float(uniforms->forward_amplitude_factor, game_state->amplitude_factor);
    shader_load_int(uniforms->forward_num_waves, game_state->num_waves);

    // FRAGMENT SHADER
    shader_load_vec3(uniforms->forward_light_dir, game_state->sun.direction);
    shader_load_vec3(uniforms->forward_light_color, game_state->sun.color);
    shader_load_float(uniforms->forward_light_intensity, game_state->sun.intensity);
    shader_load_float(uniforms->forward_ambient, game_state->ambient);
    shader_load_vec3(uniforms->forward_ambient_color, game_state->ambient_color);
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
    shader_s* shader = &game_state->forward_shader;

    // VERTEX SHADER
    uniforms->forward_time = shader_get_uniform(shader, "time");
    uniforms->forward_transformation = shader_get_uniform(shader, "transformation");
    uniforms->forward_projection_view = shader_get_uniform(shader, "projection_view");
    uniforms->forward_wavelength = shader_get_uniform(shader, "wavelength");
    uniforms->forward_amplitude = shader_get_uniform(shader, "amplitude");
    uniforms->forward_speed = shader_get_uniform(shader, "speed");
    uniforms->forward_wavelength_factor = shader_get_uniform(shader, "wavelength_factor");
    uniforms->forward_amplitude_factor = shader_get_uniform(shader, "amplitude_factor");
    uniforms->forward_num_waves = shader_get_uniform(shader, "num_waves");

    // FRAGMENT SHADER
    uniforms->forward_light_dir = shader_get_uniform(shader, "light_dir");
    uniforms->forward_light_color = shader_get_uniform(shader, "light_color");
    uniforms->forward_light_intensity = shader_get_uniform(shader, "light_intensity");
    uniforms->forward_ambient = shader_get_uniform(shader, "ambient");
    uniforms->forward_ambient_color = shader_get_uniform(shader, "ambient_color");
}
