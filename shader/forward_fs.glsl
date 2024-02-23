#version 330 core

in vec3 fs_normals;

uniform vec3 light_dir;
uniform vec3 light_color;
uniform float light_intensity;

uniform float ambient;
uniform vec3 ambient_color;

out vec4 out_color;

const vec3 water_color = vec3(0.04, 0.24, 0.96);

void main(void) {
    float diffuse_factor = max(dot(fs_normals, -normalize(light_dir)), 0.0);
    vec3 lighting = light_color * light_intensity * diffuse_factor;
    out_color = vec4(water_color * lighting + ambient_color * ambient, 1.0);
}
