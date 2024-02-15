#version 330 core

in vec3 fs_position;

uniform float blue;

out vec4 out_color;

void main(void) {
    out_color = vec4(fs_position.rg, blue, 1.0);
}
