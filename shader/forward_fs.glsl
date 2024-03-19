#version 330 core

in vec3 fs_position;
in vec3 fs_normals;
in vec3 fs_halfway_dir;
in float fs_displacement;

uniform vec3 light_dir;
uniform vec3 light_color;
uniform float light_intensity;

uniform vec3 water_color;
uniform vec3 tip_color;

uniform float tip_attenuation;

uniform float specular_factor;
uniform float specular_strength;
uniform vec3 camera_pos;

uniform float r0;

uniform float scatter_amount;
uniform float scatter_angle;
uniform vec3 scatter_color;

uniform float ambient;
uniform vec3 ambient_color;

out vec4 out_color;

float clamp01(float x) {
    return clamp(x, 0.0, 1.0);
}

void main(void) {
    vec3 color = ambient_color * ambient;

    // DIFFUSE LIGHTING
    float ndotl = dot(fs_normals, -light_dir);
    float ndotl01 = max(ndotl, 0.0);
    float diffuse_factor = max(ndotl, ambient);

    // SPECULAR LIGHTING
    vec3 camera_dir = normalize(camera_pos - fs_position);
    vec3 reflected_light = normalize(reflect(light_dir, fs_normals));
    float vdotr = dot(camera_dir, reflected_light);
    float specular_lighting = specular_strength * ndotl01 * pow(max(vdotr, 0.0), specular_factor);

    // SCHLICK FRESNEL
    float exponential = pow(1.0 - max(dot(fs_normals, fs_halfway_dir), 0.0), 5.0);
    float fresnel_factor = exponential + (1.0 - exponential) * r0;

    specular_lighting *= fresnel_factor;

    color += water_color * light_color * light_intensity * (diffuse_factor + specular_lighting);

    // TIP HIGHLIGHTING
    float tip_factor = exp(max(fs_displacement, 0.0) / tip_attenuation) - 1.0;
    vec3 tip_highlighting = tip_color * tip_factor;

    color += tip_highlighting;

    // SCATTERING
    // NOTE(nix3l): https://github.com/muckSponge/Optically-Realistic-Water/blob/master/Water/Assets/Water/Shaders/WaterSurface.shader
    // what are these values? who knows!
    float s = max(dot(reflected_light, camera_dir) * 2.0 - 1.0, 0.0);
    float sa = exp(scatter_angle * s);
    float scatter_factor = clamp01(s * scatter_amount * clamp01(ndotl * 0.7 + 0.3)) * sa;
    color += scatter_factor * mix(scatter_color * vec3(1.0, 0.4, 0.0), scatter_color, light_color);

    out_color = vec4(color, 1.0);
}
