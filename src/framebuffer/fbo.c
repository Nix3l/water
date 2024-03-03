#include "fbo.h"

#include "game.h"
#include "util/log.h"
#include "memory/memory.h"

fbo_s create_fbo(u32 width, u32 height, usize num_textures) {
    fbo_s fbo;

    glGenFramebuffers(1, &fbo.id);

    fbo.width = width;
    fbo.height = height;
    fbo.num_textures = num_textures;

    fbo.textures = arena_push(&game_state->fbo_arena, num_textures * sizeof(fbo_texture_s));
    MEM_ZERO(fbo.textures, num_textures * sizeof(fbo_texture_s));

    fbo.attachments = arena_push(&game_state->fbo_arena, num_textures * sizeof(GLenum));

    return fbo;
}

void destroy_fbo(fbo_s* fbo) {
    for(usize i = 0; i < fbo->num_textures; i ++)
        glDeleteTextures(1, &fbo->textures[i].id);

    glDeleteFramebuffers(1, &fbo->id);
}

void fbo_create_texture(fbo_s* fbo, GLenum attachment_type, GLint internal_format, GLenum format) {
    fbo_texture_s* texture = NULL;
    
    // grab the first texture that has not been initialised
    for(usize i = 0; i < fbo->num_textures; i ++) {
        if(fbo->textures[i].id == 0) {
            texture = &fbo->textures[i];

            // TODO(nix3l): not the biggest fan of putting this here
            // because it doesnt make sense but hey it works
            fbo->attachments[i] = attachment_type;
            break;
        }
    }

    if(!texture) {
        LOG_ERR("not enough space in fbo for another texture! num_textures: [%lu]\n", fbo->num_textures);
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, fbo->id);

    glGenTextures(1, &texture->id);
    texture->attachment = attachment_type;
    texture->internal_format = internal_format;
    texture->format = format;

    glBindTexture(GL_TEXTURE_2D, texture->id);

    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, fbo->width, fbo->height, 0, format, GL_FLOAT, NULL);

    // NOTE(nix3l): for now hard code these to GL_NEAREST
    // in the future (if i reuse this code) i would want to
    // have a way to set the texture parameters through a function
    // to give even more control over them
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment_type, GL_TEXTURE_2D, texture->id, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void fbo_copy_texture_to_screen(fbo_s* fbo, GLenum src_att) {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo->id);

    glReadBuffer(src_att);

    glBlitFramebuffer(0, 0, fbo->width, fbo->height, 
                      0, 0, game_state->window.width, game_state->window.height,
                      GL_COLOR_BUFFER_BIT,
                      GL_LINEAR);
    
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}
