#version 330 core

in vec2 fs_uvs;

out vec4 out_color;

void main(void) {
    out_color = vec4(fs_uvs.rg, 0.0, 1.0);
}
