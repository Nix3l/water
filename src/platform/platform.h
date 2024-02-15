#ifndef PLATFROM_LAYERH
#define PLATFROM_LAYERH

#include "base.h"
#include "memory/memory.h"

// TODO(nix3l): figure out what exactly should be in the platform layer and what should be in the application layer

char* platform_load_text_from_file(char* filepath, arena_s* arena);

#endif
