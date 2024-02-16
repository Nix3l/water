#version 330 core

layout (location = 0) in vec3 vs_position;

uniform mat4 transformation;

out vec3 fs_position;

void main(void) {
    gl_Position = transformation * vec4(vs_position, 1.0);
    fs_position = vs_position;
}
