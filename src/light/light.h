#ifndef LIGHT_H
#define LIGHT_H

#include "base.h"

typedef struct {
    v3f direction;
    f32 intensity;
    v3f color;
} directional_light_s;

#endif
