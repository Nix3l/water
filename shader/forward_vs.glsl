#version 330 core

layout (location = 0) in vec3 vs_position;
layout (location = 1) in vec2 vs_uvs;
layout (location = 2) in vec3 vs_normals;

out vec3 fs_position;

void main(void) {
    gl_Position = vec4(vs_position, 1.0);
}
