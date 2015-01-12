/**
 * @file shader.cc
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright Scry is free software released under the BSD 2-Clause license.
 */
#include "shader.h"

#include <fstream>
#include <regex>
#include "debug.h"

namespace scry {
namespace shader {

GLuint Shader(const std::string& shader_source) {
  return ShaderFromSource(shader_source);
}

GLuint ShaderFromSource(const std::string& shader_source) {
  GLuint program_id = glCreateProgram();

  GLuint vertex_shader_id = 0, geometry_shader_id = 0, fragment_shader_id = 0;

  if (shader_source.find("#ifdef VERTEX_SHADER") != std::string::npos) {
    vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    Compile(vertex_shader_id, shader_source, "VERTEX_SHADER");
    glAttachShader(program_id, vertex_shader_id);
  }
  if (shader_source.find("#ifdef GEOMETRY_SHADER") != std::string::npos) {
    geometry_shader_id = glCreateShader(GL_GEOMETRY_SHADER);
    Compile(geometry_shader_id, shader_source, "GEOMETRY_SHADER");
    glAttachShader(program_id, geometry_shader_id);
  }
  if (shader_source.find("#ifdef FRAGMENT_SHADER") != std::string::npos) {
    fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    Compile(fragment_shader_id, shader_source, "FRAGMENT_SHADER");
    glAttachShader(program_id, fragment_shader_id);
  }

  // Link
  glLinkProgram(program_id);

  _var(program_id);

  // Check
  GLint is_linked;
  int log_length;
  glGetProgramiv(program_id, GL_LINK_STATUS, &is_linked);
  glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);
  if (log_length > 0 || !is_linked) {
    std::vector<char> error_message(log_length + 1);
    glGetProgramInfoLog(program_id, log_length, nullptr, &error_message[0]);
    throw std::runtime_error(&error_message[0]);
  }

  shader_ids.push_back(program_id);

  if (vertex_shader_id != 0) glDeleteShader(vertex_shader_id);
  if (geometry_shader_id != 0) glDeleteShader(geometry_shader_id);
  if (fragment_shader_id != 0) glDeleteShader(fragment_shader_id);

  return program_id;
}

void Compile(GLuint id, const std::string& source_code,
             const std::string& shader_name) {
  // Pre-process
  auto source = std::regex_replace(source_code, std::regex("__SHADER_NAME__"),
                                   shader_name);

  // Compile
  const char* source_ptr = source.c_str();
  glShaderSource(id, 1, &source_ptr, nullptr);
  glCompileShader(id);

  // Check
  GLint is_compiled;
  int log_length;
  glGetShaderiv(id, GL_COMPILE_STATUS, &is_compiled);
  glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_length);
  if (log_length > 0 || !is_compiled) {
    std::vector<char> error_message(log_length + 1);
    glGetShaderInfoLog(id, log_length, nullptr, &error_message[0]);
    throw std::runtime_error(&error_message[0]);
  }
}

void ReadFromFile(const std::string& filename, std::string& content) {
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
}
}
