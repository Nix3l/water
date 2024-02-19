#version 330 core

layout (location = 0) in vec3 vs_position;
layout (location = 1) in vec2 vs_uvs;
layout (location = 2) in vec3 vs_normals;

uniform float time;

uniform mat4 transformation;
uniform mat4 projection_view;

out vec2 fs_uvs;

void main(void) {
    gl_Position = projection_view * transformation * vec4(vs_position, 1.0);
    fs_uvs = vs_normals.xy * vs_uvs; //* sin(time + vs_position.x + vs_position.z);
}
