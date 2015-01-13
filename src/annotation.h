/**
 * @file annotation.h
 * @brief
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
#include "shader.h"
#include "shader_object.h"
#include "graphics.h"
#include "line_shader_object.h"

namespace librender {

class Annotation {
 public:
  Annotation(float grid_z, RenderParams& render_params);
  ~Annotation();
  void Draw(const RenderParams& render_params);

  Shape* axis_data;
  Shape* grid_data;
  shader::LineShaderObject* gl_axes;
  shader::LineShaderObject* gl_grid;

  ShaderProperties shader_properties;
  GLuint line_shader;

 private:
};
}
