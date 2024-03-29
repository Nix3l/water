#version 330 core

in vec2 fs_uvs;

uniform sampler2D scene_tex;
uniform sampler2D depth_tex;

uniform float near_plane;
uniform float far_plane;

out vec4 out_color;

float get_depth() {
    float depth = texture(depth_tex, fs_uvs).r;
    float ndc = depth * 2.0 - 1.0;
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - ndc * (far_plane - near_plane));
}

void main(void) {
    vec3 color = vec3(0.0);
    float depth = get_depth();

    color += texture(scene_tex, fs_uvs).rgb;

    // FOG
    // TODO(nix3l)

    out_color = vec4(color, 1.0);
}
