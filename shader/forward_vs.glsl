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
uniform float speeds[TOTAL_WAVES];
uniform float angles[TOTAL_WAVES];

uniform float wavelength_factor;
uniform float amplitude_factor;

out vec3 fs_position;
out vec3 fs_normals;

struct wave_displacement_s {
    vec2 derivative;
    float displacement;
};

// TODO(nix3l): look into out/inout function parameters to get rid of the return value here
// NOTE(nix3l): look at https://developer.nvidia.com/gpugems/gpugems/part-i-natural-effects/chapter-1-effective-water-simulation-physical-models
wave_displacement_s calculate_wave_displacement(int index, vec2 position) {
    float angle = angles[index];
    float px = position.x * cos(angle) - position.y * sin(angle);
    float pz = position.x * sin(angle) + position.y * cos(angle); 

    vec2 derivative = vec2(0.0);
    float displacement = 0.0;

    float w = wavelengths[index]; // * pow(wavelength_factor, index);
    float a = amplitudes[index]; // * pow(amplitude_factor, index);
    for(int i = 0; i < 4; i ++) {
        float frequency = 2.0 / w;
        float phase = speeds[index] * frequency;
        float x = frequency * px + time * phase;
        float z = frequency * pz + time * phase;

        displacement += a * sin(x);
        displacement += a * sin(z);
        derivative.x += a * frequency * cos(x);
        derivative.y += a * frequency * cos(z);

        w *= wavelength_factor;
        a *= amplitude_factor;
    }

    return wave_displacement_s(derivative, displacement);
}

void main(void) {
    vec3 position = vs_position;

    float total_displacement = 0.0f;
    vec2 total_derivative = vec2(0.0);

    vec2 last_derivative = vec2(0.0);
    for(int i = 0; i < TOTAL_WAVES; i ++) {
        wave_displacement_s wave_displacement = calculate_wave_displacement(i, position.xz + last_derivative);
        last_derivative = wave_displacement.derivative;
        total_displacement += wave_displacement.displacement;
        total_derivative += wave_displacement.derivative;
    }

    position.y += total_displacement;

    // TODO(nix3l): i do not think this works lol
    /*
    vec3 tangent = normalize(vec3(1.0f, total_derivative.x, 0.0f));
    vec3 binormal = normalize(vec3(0.0f, total_derivative.y, 1.0f));
    vec3 normal = cross(binormal, tangent);
    */

    gl_Position = projection_view * transformation * vec4(position, 1.0);
    fs_position = vec3(transformation * vec4(position, 1.0));
    fs_normals = vec3(-total_derivative.x, 1.0f, -total_derivative.y);
}
