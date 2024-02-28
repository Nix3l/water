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

    forward_shader_uniforms_s* uniforms = &game_state->forward_shader_uniforms;

    // VERTEX SHADER
    shader_load_float(uniforms->time, game_state->curr_time);
    shader_load_mat4(uniforms->transformation, entity_transformation_matrix(entity));
    shader_load_mat4(uniforms->projection_view, camera_projection_view(&game_state->camera));

    for(usize i = 0; i < TOTAL_WAVES; i ++) {
        wave_s wave = game_state->waves[i];

        shader_load_float(uniforms->wavelength + i, wave.wavelength);
        shader_load_float(uniforms->amplitude  + i, wave.amplitude);
        shader_load_float(uniforms->steepness  + i, wave.steepness);
        shader_load_float(uniforms->speed      + i, wave.speed);
        shader_load_vec2 (uniforms->direction  + i, wave.direction);
    }

    shader_load_float(uniforms->wavelength_factor, game_state->wavelength_factor);
    shader_load_float(uniforms->amplitude_factor, game_state->amplitude_factor);

    // FRAGMENT SHADER
    shader_load_vec3(uniforms->light_dir, game_state->sun.direction);
    shader_load_vec3(uniforms->light_color, game_state->sun.color);
    shader_load_float(uniforms->light_intensity, game_state->sun.intensity);
    
    shader_load_vec3(uniforms->water_color, game_state->water_color);

    shader_load_float(uniforms->specular_factor, game_state->specular_factor);
    shader_load_float(uniforms->specular_strength, game_state->specular_strength);
    shader_load_vec3(uniforms->camera_pos, game_state->camera.position);

    shader_load_float(uniforms->ambient, game_state->ambient);
    shader_load_vec3(uniforms->ambient_color, game_state->ambient_color);
}

void init_forward_shader() {
    game_state->forward_shader = load_and_create_shader(
            "forward",
            "shader/forward_vs.glsl",
            "shader/forward_fs.glsl",
            bind_attributes,
            load_uniforms
        );

    forward_shader_uniforms_s* uniforms = &game_state->forward_shader_uniforms;
    shader_s* shader = &game_state->forward_shader;

    // VERTEX SHADER
    uniforms->time              = shader_get_uniform(shader, "time");
    uniforms->transformation    = shader_get_uniform(shader, "transformation");
    uniforms->projection_view   = shader_get_uniform(shader, "projection_view");

    uniforms->wavelength        = shader_get_uniform(shader, "wavelengths[0]");
    uniforms->amplitude         = shader_get_uniform(shader, "amplitudes[0]");
    uniforms->steepness         = shader_get_uniform(shader, "steepnesses[0]");
    uniforms->speed             = shader_get_uniform(shader, "speeds[0]");
    uniforms->direction         = shader_get_uniform(shader, "directions[0]");
    uniforms->wavelength_factor = shader_get_uniform(shader, "wavelength_factor");
    uniforms->amplitude_factor  = shader_get_uniform(shader, "amplitude_factor");

    // FRAGMENT SHADER
    uniforms->light_dir         = shader_get_uniform(shader, "light_dir");
    uniforms->light_color       = shader_get_uniform(shader, "light_color");
    uniforms->light_intensity   = shader_get_uniform(shader, "light_intensity");

    uniforms->water_color       = shader_get_uniform(shader, "water_color");

    uniforms->specular_factor   = shader_get_uniform(shader, "specular_factor");
    uniforms->specular_strength = shader_get_uniform(shader, "specular_strength");
    uniforms->camera_pos        = shader_get_uniform(shader, "camera_pos");
    
    uniforms->ambient           = shader_get_uniform(shader, "ambient");
    uniforms->ambient_color     = shader_get_uniform(shader, "ambient_color");
}
