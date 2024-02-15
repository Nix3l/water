#version 330 core

layout (location = 0) in vec3 vs_position;

out vec3 fs_position;

void main(void) {
    gl_Position = vec4(vs_position, 1.0);
    fs_position = vs_position;
}
