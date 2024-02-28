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
#define TOTAL_WAVES 16
uniform float wavelengths[TOTAL_WAVES];
uniform float amplitudes[TOTAL_WAVES];
uniform float steepnesses[TOTAL_WAVES];
uniform float speeds[TOTAL_WAVES];
uniform vec2 directions[TOTAL_WAVES];

uniform float wavelength_factor;
uniform float amplitude_factor;

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

    float wavelength = wavelengths[index];
    float amplitude = amplitudes[index];
    float frequency = 2.0 / wavelength;
    float phase = speeds[index] * frequency;
    float phi = time * phase;
    float steepness = steepnesses[index] / (frequency * amplitude * TOTAL_WAVES);

    vec3 displacement = calculate_displacement(xz, dir, frequency, phi, amplitude, steepness);
    vec3 normal       = calculate_normal(xz, dir, frequency, phi, amplitude, steepness);

    return wave_displacement_s(displacement, normal);
}

void main(void) {
    vec3 position = vs_position;

    vec3 total_displacement = vec3(0.0f);
    vec3 total_normal = vec3(0.0);

    vec3 last_normal = vec3(0.0);
    for(int i = 0; i < TOTAL_WAVES; i ++) {
        wave_displacement_s wave_displacement = calculate_wave_displacement(i, position + last_normal);
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
