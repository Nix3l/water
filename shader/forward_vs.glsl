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
#define TOTAL_WAVES 1
uniform float wavelengths[TOTAL_WAVES];
uniform float amplitudes[TOTAL_WAVES];
uniform float steepnesses[TOTAL_WAVES];
uniform float speeds[TOTAL_WAVES];
uniform vec2  directions[TOTAL_WAVES];
uniform float w_factors[TOTAL_WAVES];
uniform float a_factors[TOTAL_WAVES];

uniform vec2 steepness_range;
uniform float speed_ramp;
uniform float dir_angle;

uniform int num_iterations;
uniform uint seed;

out float fs_displacement;
out vec3 fs_position;
out vec3 fs_normals;

// TODO(nix3l): redo this whole thing to use better fbm

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
float to_float(uint i) {
    i &= 0x007FFFFFu;
    i |= 0x3F800000u;
    return uintBitsToFloat(i) - 1.0;
}

#define USE_GERSTNER 1

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

vec3 calculate_gerstner_derivative(float xz, vec2 dir, float f, float phi, float a, float q) {
    return vec3(0.0);
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

vec3 calculate_derivative(float xz, vec2 dir, float f, float phi, float a, float q) {
#if USE_GERSTNER
    return calculate_gerstner(xz, dir, f, phi, a, q);
#else
    return vec3(0.0);
#endif
}

// NOTE(nix3l): look at https://developer.nvidia.com/gpugems/gpugems/part-i-natural-effects/chapter-1-effective-water-simulation-physical-models
void calculate_wave_displacement(int index, vec3 position, inout vec3 displacement, inout vec3 normal) {
    vec2 dir = normalize(directions[index]);
    float xz = dir.x * position.x + dir.y * position.z;

    float w = wavelengths[index];
    float a = amplitudes[index];
    float s = speeds[index];
    float q = steepnesses[index];
    float w_factor = w_factors[index];
    float a_factor = a_factors[index];
    
    uint hash_seed = pcg_hash(seed);
    float rand = to_float(hash_seed);

    for(int i = 0; i < num_iterations; i ++) {
        float wavelength = w * pow(w_factor, i);
        float amplitude  = a * pow(a_factor, i);
        float frequency  = 2.0 / wavelength;
        float phase      = s * frequency;
        float phi        = time * phase;
        float steepness  = q / (frequency * amplitude * num_iterations);

        displacement += calculate_displacement(xz, dir, frequency, phi, amplitude, steepness);
        normal       += calculate_normal(xz, dir, frequency, phi, amplitude, steepness);
        // TODO(nix3l): derivative

        // get new random parameters for the next iteration
        hash_seed = pcg_hash(hash_seed);
        rand = to_float(hash_seed);
        dir = normalize(dir + vec2(cos(rand * dir_angle), sin(rand * dir_angle)));
        xz = dir.x * position.x + dir.y * position.z;

        s *= speed_ramp;

        hash_seed = pcg_hash(hash_seed);
        rand = to_float(hash_seed);
        q += steepness_range.x + (steepness_range.y - steepness_range.x) * rand;
    }
}

void main(void) {
    vec3 position = vs_position;

    vec3 displacement = vec3(0.0);
    vec3 normal = vec3(0.0);

    vec3 last_derivative = vec3(0.0);
    for(int i = 0; i < TOTAL_WAVES; i ++)
        calculate_wave_displacement(i, position + last_derivative, displacement, normal, last_derivative);

    position += displacement;

#if USE_GERSTNER
    normal.y = 1.0 - normal.y;
#endif

    gl_Position = projection_view * transformation * vec4(position, 1.0);
    fs_displacement = displacement.y;
    fs_position = vec3(transformation * vec4(position, 1.0));

    fs_normals = mat3(transpose(inverse(transformation))) * normalize(normal);
}
