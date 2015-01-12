/**
 * @file trimesh_shape_shader_object.cc
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-09
 * @copyright Scry is free software released under the BSD 2-Clause license.
 */
#include "trimesh_shape_shader_object.h"

namespace scry {
namespace shader {
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

TrimeshShapeShaderObject::TrimeshShapeShaderObject(
    const Shape* shape, const GLuint shader_id,
    const RenderParams& render_params) {
  glGenVertexArrays(1, &this->vertex_array_id);
  glBindVertexArray(this->vertex_array_id);

  this->shader_id = shader_id;
  this->shape = shape;

  SetAttributes(render_params, this->attribs);
  FillUniformLocations(shader_id, this->attribs);
  SetupVAO(shape);
};

/**
 * @brief
 * @param[in] render_params
 * @param[out] attribs
 */
void TrimeshShapeShaderObject::SetAttributes(
    const RenderParams& render_params,
    unordered_map<string, ShaderAttribute>& attribs) {
  auto& params = render_params.shader_params;

  attribs = {
      {"iAmbient", {AttribType::kVec3, (void*)&params.ambient[0]}},
      {"iNumLights", {AttribType::kInt, (int)params.lights.size()}},
      {"iEyeDirection", {AttribType::kVec3, (void*)&params.eye_direction[0]}},
      {"iShininess", {AttribType::kFloat, (float)params.shininess}},
      {"iStrength", {AttribType::kFloat, (float)params.strength}},
  };

  for (size_t i = 0; i < kNumMaxLights; i++) {
    string name_prefix = "iLights";
    name_prefix += "[" + std::to_string(i) + "].";

    std::unordered_map<string, ShaderAttribute> light{
        {name_prefix + "IsEnabled",
         {AttribType::kInt, (int)params.lights[i].is_enabled}},
        {name_prefix + "Color",
         {AttribType::kVec3, (void*)&params.lights[i].light_color[0]}},
        {name_prefix + "Position", 
         {AttribType::kVec3, (void*)&params.lights[i].light_position[0]}},
        {name_prefix + "ConstantAttenuation",
         {AttribType::kFloat, (float)params.lights[i].constant_attenuation}},
        {name_prefix + "LinearAttenuation",
         {AttribType::kFloat, (float)params.lights[i].linear_attenuation}},
        {name_prefix + "QuadraticAttenuation",
         {AttribType::kFloat, (float)params.lights[i].quadratic_attenuation}},
    };

    attribs.insert(light.begin(), light.end());
  }
}
}
}
