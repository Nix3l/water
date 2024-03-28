#ifndef POST_PROCESSING_SHADER_H
#define POST_PROCESSING_SHADER_H

#include "base.h"

typedef struct {
    GLuint scene;
    GLuint depth;
    GLuint near_plane;
    GLuint far_plane;
} pproc_shader_uniforms_s;

void init_post_processing_shaders();

#endif
