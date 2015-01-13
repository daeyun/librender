/**
 * @file shader.h
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright librender is free software released under the BSD 2-Clause
 * license.
 */
#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace librender {

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

GLuint Shader(const std::string& shader_source);

void ReadFromFile(const std::string& filename, std::string& content);
void Compile(GLuint id, const std::string& source_code,
             const std::string& shader_name);
GLuint ShaderFromSource(const std::string& shader_source);

static std::vector<GLuint> shader_ids;
}
}
