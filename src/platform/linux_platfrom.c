#include "platform.h"

#include <errno.h>

#include "game.h"
#include "util/log.h"

#if OS_LINUX

#define FILE_PATH_DELIM "/"

// TODO(nix3l): redo this whole thing to somehow use the executable's absolute path
// for now however i can do the hacky approach

char* platform_load_text_from_file(char* filename, usize* out_length, arena_s* arena) {
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

    if(arena->size + length >= arena->capacity && !arena->expandable) {
        LOG_ERR("not enough space in arena to output contents of file [%s]\n", filename);
        fclose(file);
        return NULL;
    }

    char* output = arena_push(arena, length + 1);
    usize read_length = fread(output, 1, length, file);
    if(read_length != length) {
        LOG_ERR("failed to read contents of [%s]: err %d\n%s\n", filename, errno, strerror(errno));
        fclose(file);
        return NULL;
    }

    output[length + 1] = '\0';

    if(out_length) *out_length = length + 1;

    fclose(file);
    return output;
}

void* platform_load_file(char* filepath, usize* buff_length, arena_s* arena) {
    FILE* file = fopen(filepath, "rb");
    if(!file) {
        LOG_ERR("failed to open [%s]: err %d\n%s\n", filepath, errno, strerror(errno));
        return NULL;
    }

    fseek(file, 0L, SEEK_END);
    usize length = ftell(file);
    rewind(file);

    if(!length) {
        LOG_ERR("failed to read length of [%s]: err %d\n%s\n", filepath, errno, strerror(errno));
        fclose(file);
        return NULL;
    }

    if(!arena_fits(arena, length)) {
        LOG_ERR("not enough space in arena to output contents of file [%s]\n", filepath);
        fclose(file);
        return NULL;
    }

    void* output = arena_push(arena, length);
    usize read_length = fread(output, 1, length, file);
    if(read_length != length) {
        LOG_ERR("failed to read contents of [%s]: err %d\n%s\n", filepath, errno, strerror(errno));
        fclose(file);
        return NULL;
    }

    if(buff_length) *buff_length = length;

    fclose(file);
    return output;
}

char* platform_get_file_extension(char* filepath) {
    char* extension = strrchr(filepath, '.');
    return extension ? extension : NULL;
}

// NOTE(nix3l): unsure whether using the std provided string functions is good or not
// but it works for now
char* platform_get_res_path(char* filename, arena_s* arena) {
    usize length = strlen(RESOURCES_PATH) + strlen(FILE_PATH_DELIM) + strlen(filename) + 1;
    char* str = arena_push(arena, length);
    strcat(str, RESOURCES_PATH);
    strcat(str, FILE_PATH_DELIM);
    strcat(str, filename);
    str[length] = '\0';
    return str;
}

#endif // OS_LINUX
