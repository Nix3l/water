#ifndef FORWARD_SHADER_H
#define FORWARD_SHADER_H

#include "base.h"

void init_forward_shader();

typedef struct {
    // FORWARD SHADER

    // VERTEX
    GLuint time;
    GLuint transformation;
    GLuint projection_view;

    // WAVE
    GLuint wavelength;
    GLuint amplitude;
    GLuint speed;
    GLuint angle;
    GLuint wavelength_factor;
    GLuint amplitude_factor;

    // FRAGMENT
    GLuint light_dir;
    GLuint light_color;
    GLuint light_intensity;
    GLuint ambient;
    GLuint ambient_color;
} forward_shader_uniforms_s;

#define TOTAL_WAVES 32

typedef struct {
    f32 wavelength;
    f32 amplitude;

    f32 speed;
    f32 angle;
} wave_s;

#endif
