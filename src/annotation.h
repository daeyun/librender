/**
 * @file annotation.h
 * @brief
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
#include "shader.h"
#include "shader_object.h"
#include "graphics.h"

namespace scry {

class Annotation {
 public:
  Annotation(float grid_z, RenderParams& render_params);
  ~Annotation();
  void draw(const RenderParams& render_params);

  Shape* axis_data;
  Shape* grid_data;
  ShaderObject gl_axes;
  ShaderObject gl_grid;

  ShaderProperties shader_properties;

 private:
};
}
