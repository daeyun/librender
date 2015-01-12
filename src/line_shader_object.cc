/**
 * @file line_shader_object.cc
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-10
 * @copyright Scry is free software released under the BSD 2-Clause license.
 */
#include "line_shader_object.h"

namespace scry {
namespace shader {
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

LineShaderObject::LineShaderObject(const Shape* shape, const GLuint shader_id,
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
void LineShaderObject::SetAttributes(
    const RenderParams& render_params,
    unordered_map<string, ShaderAttribute>& attribs) {}
}
}
