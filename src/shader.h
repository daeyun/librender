/**
 * @file shader.h
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright Scry is free software released under the BSD 2-Clause license.
 */
#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "shaders/shader_source.h"

namespace scry {

const size_t kNumMaxLights = 20;

// shader and shader attribute ids
struct GLShaderLight {
  GLuint isEnabled;
  GLuint LightColor;
  GLuint LightPosition;
  GLuint ConstantAttenuation;
  GLuint LinearAttenuation;
  GLuint QuadraticAttenuation;
};

struct GLShader {
  GLuint id;
  GLuint mvp;
  GLuint mv;
  GLuint normal_mat;
  GLuint eye_direction;
  GLuint shininess;
  GLuint strength;
  GLuint num_lights;
  GLuint ambient;
  std::vector<GLShaderLight> lights;
};

class LightProperties {
 public:
  LightProperties() {
    is_enabled = true;
    light_color = glm::vec3(1.0, 1.0, 1.0);
    light_position = glm::vec3(2, 0, 1);
    constant_attenuation = 1;
    linear_attenuation = 0.4;
    quadratic_attenuation = 0.3;
  }
  bool is_enabled;
  glm::vec3 light_color;
  glm::vec3 light_position;
  float constant_attenuation;
  float linear_attenuation;
  float quadratic_attenuation;
};

class ShaderProperties {
 public:
  ShaderProperties() {
    shininess = 20;
    strength = 1;
  }
  glm::mat4 mvp;
  glm::mat4 mv;
  glm::mat4 normal_mat;
  glm::vec3 eye_direction;
  float shininess;
  float strength;
  std::vector<LightProperties> lights;
};

namespace shader {

const std::string kVertexSuffix_ = ".v.glsl";
const std::string kFragmentSuffix_ = ".f.glsl";

GLuint Shader(const std::string& filename_prefix);
GLuint Shader(const std::string& vertex_shader_source,
              const std::string& fragment_shader_source);
GLuint Shader(const ShaderSource& shader_source);

void ReadFromFile(const std::string& filename, std::string& content);
void Compile(GLuint id, const std::string& source_code);
GLuint LinkProgram(GLuint vertex_shader_id, GLuint fragment_shader_id);
GLuint ShaderFromSource(const std::string& vertex_shader_source,
                        const std::string& fragment_shader_source);

static std::vector<GLuint> shader_ids;
}
}
