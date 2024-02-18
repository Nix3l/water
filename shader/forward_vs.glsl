#version 330 core

layout (location = 0) in vec3 vs_position;

uniform float time;

uniform mat4 transformation;
uniform mat4 projection_view;

out vec3 fs_position;

void main(void) {
    vec3 position = vs_position;

    float amplitude = 0.5;
    float lacunarity = 0.2;

    for(int i = 0; i < 4; i ++) {
        position.y += amplitude * sin(lacunarity * (time + position.x));
        position.y -= amplitude * cos(lacunarity * (time + position.z));

        amplitude *= 0.8;
        lacunarity *= 1.5;
    }

    gl_Position = projection_view * transformation * vec4(position, 1.0);
    fs_position = vs_position;
}
