#version 330 core

in float displacement;
in vec3 fs_position;
in vec3 fs_normals;

uniform vec3 light_dir;
uniform vec3 light_color;
uniform float light_intensity;

uniform vec3 water_color = vec3(0.04, 0.24, 0.96);
uniform vec3 tip_color = vec3(1.0, 1.0, 1.0);

uniform float tip_attenuation = 18.0;

uniform float specular_factor = 32.0;
uniform float specular_strength = 1.0;
uniform vec3 camera_pos;

uniform float r0 = 1.33f;

uniform float ambient = 0.24;
uniform vec3 ambient_color;

out vec4 out_color;

void main(void) {
    // DIFFUSE LIGHTING
    float ndotl = max(dot(fs_normals, -light_dir), 0.0);
    float diffuse_factor = max(ndotl, ambient);

    // SPECULAR LIGHTING
    vec3 camera_dir = normalize(camera_pos - fs_position);
    vec3 reflected_light = normalize(reflect(light_dir, fs_normals));
    float specular_lighting = specular_strength * ndotl * pow(max(dot(camera_dir, reflected_light), 0.0), specular_factor);

    // SCHLICK FRESNEL
    vec3 halfway_dir = camera_dir + light_dir;
    float exponential = pow(1.0 - max(dot(fs_normals, halfway_dir), 0.0), 5.0);
    float fresnel_factor = exponential + (1.0 - exponential) * r0;

    specular_lighting *= fresnel_factor;

    // TOTAL LIGHTING
    vec3 lighting = light_color * light_intensity * (diffuse_factor + specular_lighting);

    // TIP HIGHLIGHTING
    float tip_factor = exp(max(displacement, 0.0) / tip_attenuation) - 1;
    vec3 tip_highlighting = tip_color * tip_factor;

    out_color = vec4(water_color * lighting + ambient_color * ambient + tip_highlighting, 1.0);
}
