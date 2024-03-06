#include "params.h"

#include "game.h"
#include "platform/platform.h"
#include "util/log.h"

#define WAVE_IDENTIFIER "wave"

#define WAVELENGTH "wavelength"
#define AMPLITUDE  "amplitude"
#define STEEPNESS  "steepness"
#define DIRECTION  "direction"
#define W_FACTOR   "w_factor"
#define A_FACTOR   "a_factor"

#define FMT_FLT  "%f"
#define FMT_FLT2 "%f, %f"

#define GET_WAVE_FLOAT(_wave, _param, _name, _val) do { \
    if(strncmp((_name), #_param, strlen(#_param)) == 0) \
        sscanf((_val), FMT_FLT, &wave->_param); \
} while(0)

#define GET_WAVE_FLOAT2(_wave, _param, _name, _val) do { \
    if(strncmp((_name), #_param, strlen(#_param)) == 0) \
        sscanf((_val), FMT_FLT2, &wave->_param.x, &wave->_param.y); \
} while(0)

static usize load_wave(char** lines, usize start_index, usize wave_index) {
    wave_s* wave = &game_state->waves[wave_index];

    char* line;
    while((line = lines[++start_index])) {
        LOG("%s\n", line);
        if(*line == '}') return start_index;

        char* name;
        char* value;
        sscanf(line, "\t%s %[^]]", name, value);

        // thank you preprocessor
        GET_WAVE_FLOAT (wave, wavelength, name, value);
        GET_WAVE_FLOAT (wave, amplitude,  name, value);
        GET_WAVE_FLOAT (wave, steepness,  name, value);
        GET_WAVE_FLOAT (wave, speed,      name, value);
        GET_WAVE_FLOAT2(wave, direction,  name, value);
        GET_WAVE_FLOAT (wave, w_factor,   name, value);
        GET_WAVE_FLOAT (wave, a_factor,   name, value);
    }

    // there should not be a situation in which this is reached
    LOG_WARN("reached unreachable part of function, there might be a problem in the parameters file.");
    return start_index;
}

void load_parameters_from_file(char* filepath) {
    arena_s temp_mem = arena_create_in_block(game_memory->transient_storage, game_memory->transient_storage_size);
    
    usize num_lines = 0;
    char** lines = platform_load_lines_from_file(filepath, &num_lines, &temp_mem);

    if(num_lines == 0) return;

    for(usize i = 0; i < num_lines; i ++)
        LOG("%lu] %s\n", i, lines[i]);

    for(usize i = 0; i < num_lines; i ++) {
        char* line = lines[i];

        if(strncmp(line, WAVE_IDENTIFIER, strlen(WAVE_IDENTIFIER)) == 0) {
            u32 wave_index;
            sscanf(line, "wave %u {", &wave_index);
            // assign i to the return value of this function
            // to pick up where it left off
            i = load_wave(lines, i, wave_index);
        }
    }
}
