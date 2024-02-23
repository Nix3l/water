#version 330 core

layout (location = 0) in vec3 vs_position;
layout (location = 1) in vec2 vs_uvs;
layout (location = 2) in vec3 vs_normals;

uniform float time;

uniform float wavelength;
uniform float amplitude;
uniform float speed;

uniform float wavelength_factor;
uniform float amplitude_factor;

uniform int num_waves;

uniform mat4 transformation;
uniform mat4 projection_view;

out vec2 fs_uvs;

void main(void) {
    vec3 position = vs_position;

    // TODO(nix3l): lighting + normals

    float w = wavelength;
    float a = amplitude;
    for(int i = 0; i < num_waves; i ++) {
        float frequency = 2.0 / w;
        float phase = speed * frequency;
        position.y += a * sin(frequency * position.x + time * phase);
        position.y += a * sin(frequency * position.z + time * phase);

        w *= wavelength_factor;
        a *= amplitude_factor;
    }

    gl_Position = projection_view * transformation * vec4(position, 1.0);
    fs_uvs = vec2(sin(position.x + time), sin(position.z + time));
}
