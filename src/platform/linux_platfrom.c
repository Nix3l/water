#include "platform.h"

#include <errno.h>

#include "game.h"
#include "util/log.h"

#if OS_LINUX

// TODO(nix3l): redo this whole thing to somehow use the executable's absolute path
// for now however i can do the hacky approach

char* platform_load_text_from_file(char* filename, usize* out_length, arena_s* arena) {
    char* temp = game_memory->transient_storage;

    FILE* file = fopen(filename, "rb");
    if(!file) {
        LOG_ERR("failed to open [%s]: err %d\n%s\n", filename, errno, strerror(errno));
        return NULL;
    }

    fseek(file, 0L, SEEK_END);
    usize length = ftell(file);
    rewind(file);

    if(!length) {
        LOG_ERR("failed to read length of [%s]: err %d\n%s\n", filename, errno, strerror(errno));
        fclose(file);
        return NULL;
    }

    ASSERT(length <= game_memory->transient_storage_size);

    if(arena->size + length >= arena->capacity && !arena->expandable) {
        LOG_ERR("not enough space in arena to output contents of file [%s]\n", filename);
        fclose(file);
        return NULL;
    }

    MEM_ZERO(temp, length + 1);
    usize read_length = fread(temp, 1, length, file);
    if(read_length != length) {
        LOG_ERR("failed to read contents of [%s]: err %d\n%s\n", filename, errno, strerror(errno));
        fclose(file);
        return NULL;
    }

    char* output = arena_push(arena, length + 1);
    strncpy(output, temp, length);
    output[length + 1] = '\0';

    if(out_length) *out_length = length + 1;

    fclose(file);
    return output;
}

#endif // OS_LINUX
