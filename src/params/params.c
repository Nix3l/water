#include "params.h"

#include "game.h"
#include "platform/platform.h"
#include "util/log.h"

#define WAVE_IDENTIFIER "wave"

#define FMT_U32  "%u"
#define FMT_FLT  "%f"
#define FMT_FLT2 "%f, %f"
#define FMT_FLT3 "%f, %f, %f"

#define GET_PARAM_U32(_parent, _param, _name, _val) do { \
    if(strncmp((_name), #_param, strlen(#_param)) == 0) \
        sscanf((_val), FMT_U32, &(_parent)->_param); \
} while(0)

#define GET_PARAM_FLT(_parent, _param, _name, _val) do { \
    if(strncmp((_name), #_param, strlen(#_param)) == 0) \
        sscanf((_val), FMT_FLT, &(_parent)->_param); \
} while(0)

#define GET_PARAM_FLT2(_parent, _param, _name, _val) do { \
    if(strncmp((_name), #_param, strlen(#_param)) == 0) \
        sscanf((_val), FMT_FLT2, &(_parent)->_param.x, &(_parent)->_param.y); \
} while(0)

#define GET_PARAM_FLT3(_parent, _param, _name, _val) do { \
    if(strncmp((_name), #_param, strlen(#_param)) == 0) \
        sscanf((_val), FMT_FLT3, &(_parent)->_param.x, &(_parent)->_param.y, &(_parent)->_param.z); \
} while(0)

static usize load_wave(char** lines, usize i, usize wave_index) {
    wave_s* wave = &game_state->waves[wave_index];

    char* line;
    while((line = lines[++i])) {
        if(*line == '}') return i;

        char name[32];
        char value[32];
        sscanf(line, "\t%s [%[^]]", name, value);

        // thank you preprocessor
        GET_PARAM_FLT (wave, wavelength, name, value);
        GET_PARAM_FLT (wave, amplitude,  name, value);
        GET_PARAM_FLT (wave, steepness,  name, value);
        GET_PARAM_FLT (wave, speed,      name, value);
        GET_PARAM_FLT2(wave, direction,  name, value);
        GET_PARAM_FLT (wave, w_factor,   name, value);
        GET_PARAM_FLT (wave, a_factor,   name, value);
    }

    // there should not be a situation in which this is reached
    LOG_WARN("reached unreachable part of function, there might be a problem in the parameters file.");
    return i;
}

static void load_param(char* line) {
    char name[32];
    char val[32];
    sscanf(line, "%s [%[^]]", name, val);

    // TODO(nix3l): add all the serialised parameters here
    GET_PARAM_U32(game_state, num_iterations, name, val);
    GET_PARAM_U32(game_state, seed,           name, val);
}

void load_parameters_from_file(char* filepath, arena_s* arena) {
    usize num_lines = 0;
    char** lines = platform_load_lines_from_file(filepath, &num_lines, arena);

    if(num_lines == 0) return;

    for(usize i = 0; i < num_lines; i ++) {
        char* line = lines[i];

        // wave blocks
        if(strncmp(line, WAVE_IDENTIFIER, strlen(WAVE_IDENTIFIER)) == 0) {
            u32 wave_index;
            sscanf(line, "wave %u {", &wave_index);

            // TODO(nix3l): skipping an extra wave block doesnt work yet

            // if there are more waves than expected,
            // skip the current wave block and process
            // whatever comes afterwards
            if(wave_index >= TOTAL_WAVES) {
                while(i < num_lines)
                    if(*lines[i++] == '}') continue;

                // if we reach the end of the file for whatever reason, break
                LOG_WARN("reached end of file when trying to skip extra wave block");
                break;
            }

            // assign i to the return value of this function
            // to pick up where it left off
            i = load_wave(lines, i, wave_index);
            continue;
        }

        load_param(line);
    }
}
