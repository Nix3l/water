#version 330 core

in vec3 fs_position;

out vec4 out_color;

void main(void) {
    out_color = vec4(fs_position.rg, 0.3f, 1.0);
}
