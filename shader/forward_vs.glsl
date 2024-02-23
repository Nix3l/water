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

out vec3 fs_normals;

void main(void) {
    vec3 position = vs_position;

    // TODO(nix3l): lighting + normals

    float w = wavelength;
    float a = amplitude;
    
    float dx = 0.0f; 
    float dz = 0.0f; 

    for(int i = 0; i < num_waves; i ++) {
        float frequency = 2.0 / w;
        float phase = speed * frequency;
        position.y += a * sin(frequency * position.x + time * phase);
        position.y += a * sin(frequency * position.z + time * phase);

        dx += a * frequency * cos(frequency * position.x + time * phase);
        dz += a * frequency * cos(frequency * position.z + time * phase);

        w *= wavelength_factor;
        a *= amplitude_factor;
    }

    vec3 tangent  = normalize(vec3(1.0f, dx, 0.0f));
    vec3 binormal = normalize(vec3(0.0f, dz, 1.0f));
    vec3 normal = -cross(tangent, binormal);

    gl_Position = projection_view * transformation * vec4(position, 1.0);
    fs_normals = normal;
}
