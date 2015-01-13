/**
 * @file trimesh_normal_shader_object.cc
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-09
 * @copyright librender is free software released under the BSD 2-Clause
 * license.
 */
#include "trimesh_normal_shader_object.h"

namespace librender {
namespace shader {
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

TrimeshNormalShaderObject::TrimeshNormalShaderObject(
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
void TrimeshNormalShaderObject::SetAttributes(
    const RenderParams& render_params,
    unordered_map<string, ShaderAttribute>& attribs) {
  auto& params = render_params.shader_params;

  attribs = {
      {"iFaceNormalColor",
       {AttribType::kVec4, (void*)&params.face_normal_color[0]}},
      {"iFaceNormalLength",
       {AttribType::kFloat, (float)params.face_normal_length}},
  };
}
}
}
