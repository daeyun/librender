/**
 * @file graphics.h
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright Scry is free software released under the BSD 2-Clause license.
 */
#pragma once

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "shape.h"
#include "shader.h"

namespace scry {

enum Axis { X, Y, Z };

struct ShaderParams {
  glm::mat4 view_mat;
  glm::mat4 projection_mat;
  glm::mat4 model_mat;
  glm::vec3 eye_direction;

  std::vector<LightProperties> lights;

  float shininess;
  float strength;
  glm::vec3 ambient;
};

class RenderParams {
  public:
    RenderParams();

    bool will_normalize;
    glm::vec3 target;
    float fov;
    Axis up_axis;
    bool are_axes_visible;
    float up_angle;
    float el;
    float az;
    float r;
    float near;
    float far;
    int image_width;
    int image_height;
    int num_msaa_samples;
    glm::vec4 background;
    arma::fvec color;
    bool is_color_forced;

    std::string in_filename;
    std::string out_filename;
    bool can_overwrite;

    ShaderParams shader_params;
};

const float kPi = glm::pi<float>();

void Render(const Shape& shape, RenderParams& params);
void RotateVector(const glm::vec3& axis, const float angle, glm::vec3& vector);
void ComputeMatrices(RenderParams& render_params);
}
