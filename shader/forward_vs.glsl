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
#define TOTAL_WAVES 8
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
wave_displacement_s calculate_wave_displacement(int index, vec3 position) {
    float angle = angles[index];
    float x = position.x * cos(angle) - position.z * sin(angle);
    float z = position.x * sin(angle) + position.z * cos(angle); 

    vec2 derivative = vec2(0.0);
    float displacement = 0.0;

    float a = amplitudes[index] * pow(amplitude_factor, index);
    float w = wavelengths[index] * pow(wavelength_factor, index);
    float frequency = 2.0 / w;
    float phase = speeds[index] * frequency;
    displacement += a * sin(frequency * x + time * phase);
    displacement += a * sin(frequency * z + time * phase);

    derivative.x += a * frequency * cos(frequency * x + time * phase);
    derivative.y += a * frequency * cos(frequency * z + time * phase);

    return wave_displacement_s(derivative, displacement);
}

void main(void) {
    vec3 position = vs_position;

    // TODO(nix3l): lighting + normals

    float total_displacement = 0.0f;
    vec2 total_derivative = vec2(0.0);

    for(int i = 0; i < TOTAL_WAVES; i ++) {
        wave_displacement_s wave_displacement = calculate_wave_displacement(i, position);
        total_displacement += wave_displacement.displacement;
        total_derivative += wave_displacement.derivative;
    }

    position.y += total_displacement;

    // TODO(nix3l): i do not think this works lol
    vec3 tangent = normalize(vec3(1.0f, total_derivative.x, 0.0f));
    vec3 binormal = normalize(vec3(0.0f, total_derivative.y, -1.0f));
    vec3 normal = cross(tangent, binormal);

    gl_Position = projection_view * transformation * vec4(position, 1.0);
    fs_position = vec3(transformation * vec4(position, 1.0));
    fs_normals = normal;
}
