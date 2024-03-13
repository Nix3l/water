#ifndef WATER_SHADER_H
#define WATER_SHADER_H

#include "base.h"

#define TOTAL_WAVES 1

void init_water_shader();

typedef struct {
    // VERTEX
    GLuint time;
    GLuint transformation;
    GLuint projection_view;

    // WAVE
    GLuint wavelength;
    GLuint amplitude;
    GLuint steepness;
    GLuint speed;
    GLuint direction;
    GLuint w_factor;
    GLuint a_factor;

    GLuint steepness_range;
    GLuint speed_ramp;
    GLuint dir_angle;

    GLuint num_iterations;
    GLuint seed;

    // FRAGMENT
    GLuint light_dir;
    GLuint light_color;
    GLuint light_intensity;

    GLuint water_color;

    GLuint tip_color;
    GLuint tip_attenuation;

    GLuint specular_factor;
    GLuint specular_strength;
    GLuint camera_pos;

    GLuint r0;

    GLuint ambient;
    GLuint ambient_color;
} water_shader_uniforms_s;

typedef struct {
    f32 wavelength;
    f32 amplitude;
    f32 steepness;

    f32 speed;
    v2f direction;

    f32 w_factor;
    f32 a_factor;
} wave_s;

#endif
