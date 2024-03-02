#ifndef FBO_H
#define FBO_H

#include "base.h"

typedef struct {
    GLuint id;
    GLenum attachment;

    GLint internal_format;
    GLenum format;
} fbo_texture_s;

typedef struct {
    GLuint id;

    u32 width;
    u32 height;

    usize num_textures;
    fbo_texture_s* textures;

    GLenum* attachments;
} fbo_s;

fbo_s create_fbo(u32 width, u32 height, usize num_textures);
void destroy_fbo(fbo_s* fbo);

void fbo_create_texture(fbo_s* fbo, GLenum attachment_type, GLint internal_format, GLenum format);

void fbo_copy_texture_to_screen(fbo_s* fbo, GLenum src_att);

#endif
