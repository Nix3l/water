#ifndef WATER_SHADER_H
#define WATER_SHADER_H

#include "base.h"

#define TOTAL_WAVES 12

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

    GLuint refractive_index;

    GLuint ambient;
    GLuint ambient_color;
} water_shader_uniforms_s;

typedef struct {
    f32 wavelength;
    f32 amplitude;
    f32 steepness;

    f32 speed;
    v2f direction;
} wave_s;

#endif
