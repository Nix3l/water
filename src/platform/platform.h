#ifndef PLATFROM_LAYERH
#define PLATFROM_LAYERH

#include "base.h"
#include "memory/memory.h"

// TODO(nix3l): figure out what exactly should be in the platform layer and what should be in the application layer

// returns all the text in a given filepath. length_read can be null if the length of the file is not needed
char* platform_load_text_from_file(char* filepath, usize* length_read, arena_s* arena);

#endif
