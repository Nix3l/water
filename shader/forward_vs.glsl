#version 330 core

layout (location = 0) in vec3 vs_position;
layout (location = 1) in vec2 vs_uvs;
layout (location = 2) in vec3 vs_normals;

uniform float time;

uniform mat4 transformation;
uniform mat4 projection_view;

// NOTE(nix3l): had to change this from using a struct array
// to using separate arrays for each parameter
// because glsl is stupid when it comes to accessing structs in uniforms

// WAVE DATA
#define TOTAL_WAVES 3
uniform float wavelengths[TOTAL_WAVES];
uniform float amplitudes[TOTAL_WAVES];
uniform float steepnesses[TOTAL_WAVES];
uniform float speeds[TOTAL_WAVES];
uniform vec2  directions[TOTAL_WAVES];
uniform float w_factors[TOTAL_WAVES];
uniform float a_factors[TOTAL_WAVES];

uniform vec2 steepness_range;
uniform vec2 speed_range;

uniform int num_iterations = 4;
uniform uint seed;

uniform float push_strength = 1.0;

// NOTE(nix3l): also see https://amindforeverprogramming.blogspot.com/2013/07/random-floats-in-glsl-330.html
// for more info on random in shaders 

// NOTE(nix3l): no clue how this works but if you care
// https://jcgt.org/published/0009/03/02/paper.pdf
uint pcg_hash(uint input) {
    uint state = input * 747796405u + 2891336453u;
    uint word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return (word >> 22u) ^ word;
}

// take advantage of the IEEE standard to get some floats from the hash function
float randf(float f) {
    const uint mantissa = 0x007FFFFFu;
    const uint one      = 0x3F800000u;

    uint hash = pcg_hash(floatBitsToUint(f));
    hash &= mantissa;
    hash |= one;

    return uintBitsToFloat(hash) - 1.0;
}
out float displacement;
out vec3 fs_position;
out vec3 fs_normals;

#define USE_GERSTNER 1

struct wave_displacement_s {
    vec3 displacement;
    vec3 normal;
};

vec3 calculate_gerstner(float xz, vec2 dir, float f, float phi, float a, float q) {
    vec3 gerstner = vec3(0.0);

    gerstner.x = q * a * dir.x * cos(f * xz + phi);
    gerstner.y = a * sin(f * xz + phi);
    gerstner.z = q * a * dir.y * cos(f * xz + phi);

    return gerstner;
}

vec3 calculate_gerstner_normal(float xz, vec2 dir, float f, float phi, float a, float q) {
    vec3 normal = vec3(0.0);

    float fa = f * a;
    float s = sin(f * xz + phi);
    float c = cos(f * xz + phi);

    normal.x = -(dir.x * fa * c);
    normal.y = q * fa * s;
    normal.z = -(dir.y * fa * c);

    return normal;
}

vec3 calculate_normal(float xz, vec2 dir, float f, float phi, float a, float q) {
#if USE_GERSTNER
    return calculate_gerstner_normal(xz, dir, f, phi, a, q);
#else
    return vec3(0.0);
#endif
}

vec3 calculate_displacement(float xz, vec2 dir, float f, float phi, float a, float q) {
#if USE_GERSTNER
    return calculate_gerstner(xz, dir, f, phi, a, q);
#else
    return vec3(0.0);
#endif
}

// TODO(nix3l): look into out/inout function parameters to get rid of the return value here
// NOTE(nix3l): look at https://developer.nvidia.com/gpugems/gpugems/part-i-natural-effects/chapter-1-effective-water-simulation-physical-models
wave_displacement_s calculate_wave_displacement(int index, vec3 position) {
    vec2 dir = normalize(directions[index]);
    float xz = dir.x * position.x + dir.y * position.z;

    float w = wavelengths[index];
    float a = amplitudes[index];
    float s = speeds[index];
    float q = steepnesses[index];
    float w_factor = w_factors[index];
    float a_factor = a_factors[index];
    
    uint hash_seed = pcg_hash(seed);
    float rand = randf(s);

    wave_displacement_s output = wave_displacement_s(vec3(0.0), vec3(0.0));

    for(int i = 0; i < num_iterations; i ++) {
        float wavelength = w * pow(w_factor, i);
        float amplitude  = a * pow(a_factor, i);
        float frequency  = 2.0 / wavelength;
        float phase      = s * frequency;
        float phi        = time * phase;
        float steepness  = q / (frequency * amplitude * num_iterations);

        output.displacement += calculate_displacement(xz, dir, frequency, phi, amplitude, steepness);
        // TODO(nix3l): add threshold for wavelength (so very small waves dont affect)
        output.normal       += calculate_normal(xz, dir, frequency, phi, amplitude, steepness);

        // get new random parameters for the next iteration
        hash_seed = pcg_hash(hash_seed);
        rand = randf(hash_seed);

        dir = normalize(vec2(cos(rand), sin(rand)));
        xz = dir.x * position.x + dir.y * position.z;

        hash_seed = pcg_hash(hash_seed);
        rand = randf(hash_seed);

        s += speed_range.x + (speed_range.y - speed_range.x) * rand;

        hash_seed = pcg_hash(hash_seed);
        rand = randf(hash_seed);
        
        q += steepness_range.x + (steepness_range.y - steepness_range.x) * rand;
    }

    return output;
}

void main(void) {
    vec3 position = vs_position;

    vec3 total_displacement = vec3(0.0f);
    vec3 total_normal = vec3(0.0);

    vec3 last_normal = vec3(0.0);
    for(int i = 0; i < TOTAL_WAVES; i ++) {
        wave_displacement_s wave_displacement = calculate_wave_displacement(i, position + last_normal * push_strength);
        last_normal = wave_displacement.normal;
        total_displacement += wave_displacement.displacement;
        total_normal += wave_displacement.normal;
    }

    position += total_displacement;

    total_normal.y = 1.0 - total_normal.y;

    gl_Position = projection_view * transformation * vec4(position, 1.0);
    displacement = total_displacement.y;
    fs_position = vec3(transformation * vec4(position, 1.0));
    fs_normals = normalize(total_normal);
}
