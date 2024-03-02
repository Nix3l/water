#ifndef RENDERER_H
#define RENDERER_H

#include "entity/entity.h"
#include "shader/shader.h"
#include "framebuffer/fbo.h"

// TODO(nix3l): deferred rendering

typedef struct {
    v4f background_color;
    shader_s* shader;

    fbo_s framebuffer;

    bool render_wireframe;
} forward_renderer_s;

void init_forward_renderer();

void render_forward(entity_s* entity);

#endif
