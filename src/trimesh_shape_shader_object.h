/**
 * @file trimesh_shape_shader_object.h
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-09
 * @copyright Scry is free software released under the BSD 2-Clause license.
 */
#pragma once

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "shape.h"
#include "databuffer.h"
#include "shader.h"
#include "shaders/trimesh_shape_shader.h"
#include "shader_object.h"

namespace scry {
namespace shader {
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;
using std::vector;

class TrimeshShapeShaderObject : public ShaderObject {
 public:
  TrimeshShapeShaderObject() = default;
  TrimeshShapeShaderObject(const Shape* shape, const GLuint shader_id,
                           const RenderParams& render_params);

 private:
  void SetAttributes(const RenderParams& render_params,
                     unordered_map<string, ShaderAttribute>& attribs) override;
};
}
}
