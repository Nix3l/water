#version 330 core

in vec3 fs_position;
in vec3 fs_normals;

uniform vec3 light_dir;
uniform vec3 light_color;
uniform float light_intensity;

uniform vec3 water_color = vec3(0.04, 0.24, 0.96);

uniform float specular_factor = 8.0;
uniform float specular_strength = 0.5;
uniform vec3 camera_pos;

uniform float ambient = 0.1;
uniform vec3 ambient_color;

out vec4 out_color;

void main(void) {
    // DIFFUSE LIGHTING
    float diffuse_factor = max(dot(fs_normals, -light_dir), 0.0);

    // SPECULAR LIGHTING
    vec3 camera_dir = normalize(camera_pos - fs_position);
    vec3 reflected_light = normalize(reflect(light_dir, fs_normals));
    float specular_lighting = specular_strength * pow(max(dot(camera_dir, reflected_light), 0.0), specular_factor);

    // TOTAL LIGHTING
    // TODO(nix3l): move the ambient lighting to be in this variable instead
    vec3 lighting = light_color * light_intensity * (diffuse_factor + specular_lighting);

    out_color = vec4(water_color * lighting + ambient_color * ambient, 1.0);
}
