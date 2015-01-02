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

struct CameraParams {
  glm::vec3 target;
  float el;
  float az;
  float r;
  float up_ang;
};

struct ShaderViewParams {
  glm::mat4 view_mat;
  glm::mat4 projection_mat;
  glm::mat4 model_mat;
  glm::vec3 eye_direction;
};

const float kPi = glm::pi<float>();

void Render(const Shape& object, const std::string& filename = "");
void InitializeShader(ShaderProperties& shader_properties);
void RotateVector(const glm::vec3& axis, const float angle, glm::vec3& vector);
void ComputeMatrices(const CameraParams& params, ShaderViewParams& output);
void SaveAsPNG(const std::string& filename, const uint8_t* data, int w, int h);
void UpdateShaderView(const ShaderViewParams& view_params,
                      ShaderProperties& shader_properties);
}
