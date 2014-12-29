#include "shader.h"

#include <string>
#include <vector>
#include <fstream>
#include <GL/glew.h>
#include "shaders/shader_source.h"

GLuint Shader::Shader(const std::string& filename_prefix) {
    std::string vertex_filename = filename_prefix + kVertexSuffix_;
    std::string fragment_filename = filename_prefix + kFragmentSuffix_;

    std::string vertex_source;
    std::string fragment_source;

    read_from_file(vertex_filename, vertex_source);
    read_from_file(fragment_filename, fragment_source);

    return shader_from_source(vertex_source, fragment_source);
}

GLuint Shader::Shader(const std::string& vertex_shader_source,
                      const std::string& fragment_shader_source) {
    return shader_from_source(vertex_shader_source, fragment_shader_source);
}

GLuint Shader::Shader(const ShaderSource& shader_source) {
    return shader_from_source(shader_source.vertex, shader_source.fragment);
}

GLuint Shader::shader_from_source(const std::string& vertex_shader_source,
                                  const std::string& fragment_shader_source) {
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    compile(vertex_shader_id, vertex_shader_source);
    compile(fragment_shader_id, fragment_shader_source);

    GLuint program_id = link_program(vertex_shader_id, fragment_shader_id);
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return program_id;
}

void Shader::compile(GLuint id, const std::string& source_code) {
    // Compile
    char const* source_ptr = source_code.c_str();
    glShaderSource(id, 1, &source_ptr, NULL);
    glCompileShader(id);

    // Check
    GLint is_compiled;
    int log_length;
    glGetShaderiv(id, GL_COMPILE_STATUS, &is_compiled);
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_length);
    if (log_length > 0 || !is_compiled) {
        std::vector<char> error_message(log_length + 1);
        glGetShaderInfoLog(id, log_length, NULL, &error_message[0]);
        throw std::runtime_error(&error_message[0]);
    }
}

GLuint Shader::link_program(GLuint vertex_shader_id,
                            GLuint fragment_shader_id) {
    // Link
    GLuint program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);
    glLinkProgram(program_id);

    // Check
    GLint is_linked;
    int log_length;
    glGetProgramiv(program_id, GL_LINK_STATUS, &is_linked);
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);
    if (log_length > 0 || !is_linked) {
        std::vector<char> error_message(log_length + 1);
        glGetProgramInfoLog(program_id, log_length, NULL, &error_message[0]);
        throw std::runtime_error(&error_message[0]);
    }

    shader_ids.push_back(program_id);

    return program_id;
}

void Shader::read_from_file(const std::string& filename, std::string& content) {
    // Read the Vertex Shader code from the file.
    std::ifstream VertexShaderStream(filename, std::ios::in);
    if (VertexShaderStream.is_open()) {
        std::string line;
        while (getline(VertexShaderStream, line)) content += line + "\n";
        VertexShaderStream.close();
    } else {
        std::string error_message("Cannot open ");
        error_message += filename;
        throw std::runtime_error(error_message);
    }
}
