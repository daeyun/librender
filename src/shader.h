#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "shaders/shader_source.h"

static GLuint scry_current_shader = 0;

const size_t kNumMaxLights = 20;

// shader and shader attribute ids
struct GLShaderLight {
    GLuint isEnabled;
    GLuint Ambient;
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
    std::vector<GLShaderLight> lights;
};
class LightProperties {
   public:
    LightProperties() {
        is_enabled = true;
        ambient = glm::vec3(0.2, 0.2, 0.2);
        light_color = glm::vec3(1.0, 1.0, 1.0);
        light_position = glm::vec3(3.0, 3.0, 3.0);
        constant_attenuation = 1;
        linear_attenuation = 0;
        quadratic_attenuation = 0;
    }
    bool is_enabled;
    glm::vec3 ambient;
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

namespace Shader {

static std::vector<GLuint> shader_ids;

const std::string kVertexSuffix_ = ".v.glsl";
const std::string kFragmentSuffix_ = ".f.glsl";

GLuint Shader(const std::string& filename_prefix);
GLuint Shader(const std::string& vertex_shader_source,
              const std::string& fragment_shader_source);
GLuint Shader(const ShaderSource& shader_source);

void read_from_file(const std::string& filename, std::string& content);

void compile(GLuint id, const std::string& source_code);

GLuint link_program(GLuint vertex_shader_id, GLuint fragment_shader_id);

GLuint shader_from_source(const std::string& vertex_shader_source,
                          const std::string& fragment_shader_source);
};
