#include "shader.h"

#include "game.h"
#include "util/log.h"

static GLuint compile_shader(char* src_code, GLuint shader_type) {
    GLuint id = glCreateShader(shader_type);

    glShaderSource(id, 1, (const char* const*)&src_code, NULL);
    glCompileShader(id);

    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);

    if(!success) {
        // TODO(nix3l): kinda dumb to have this have a fixed size like this.
        // is there a better solution?
        char log[512];
        glGetShaderInfoLog(id, sizeof(log), NULL, log);
        LOG_ERR("failed to compile shader:\n%s\n", log);
    }

    return id;
}

shader_s create_shader(char* filename, char* vertex_src, char* fragment_src, void (*bind_attributes) ()) {
    shader_s shader;

    shader.name = filename;
    shader.vertex_full_path = NULL;
    shader.fragment_full_path = NULL;

    shader.program_id = glCreateProgram();

    shader.vertex_id = compile_shader(vertex_src, GL_VERTEX_SHADER);
    glAttachShader(shader.program_id, shader.vertex_id);

    shader.fragment_id = compile_shader(fragment_src, GL_FRAGMENT_SHADER);
    glAttachShader(shader.program_id, shader.fragment_id);

    bind_attributes();

    glLinkProgram(shader.program_id);
    glValidateProgram(shader.program_id);

    int success;
    glGetProgramiv(shader.program_id, GL_LINK_STATUS, &success);

    if(!success) {
        char log[512];
        glGetProgramInfoLog(shader.program_id, 512, NULL, log);
        LOG_ERR("failed to link shader:\n%s\n", log);
    }

    // TODO(nix3l): get the uniform locations

    // so this is a bit confusing due to the way it works in opengl
    // basically, when a vs/fs/whatever shader is compiled it is its own thing
    // separate from the shader program (pipeline really)
    // after attaching the shader to the program the old compiled shaders still remain
    // so we are wasting memory by keeping them around
    // when they are already linked to the program
    // hence, delete them to free up gpu resources
    glDetachShader(shader.program_id, shader.vertex_id);
    glDetachShader(shader.program_id, shader.fragment_id);

    glDeleteShader(shader.vertex_id);
    glDeleteShader(shader.fragment_id);

    return shader;
}

void destroy_shader(shader_s* shader) {
    glDeleteProgram(shader->program_id);
}

void shader_start(shader_s* shader) {
    glUseProgram(shader->program_id);
}

void shader_stop() {
    glUseProgram(0);
}
