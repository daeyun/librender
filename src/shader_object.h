/**
 * @file shader_object.h
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright Scry is free software released under the BSD 2-Clause license.
 */
#pragma once

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "shape.h"
#include "databuffer.h"
#include "shader.h"
#include "graphics.h"

namespace scry {

class ShaderObject {
 public:
  ShaderObject();
  ShaderObject(const Shape* shape, const GLuint shader_id,
               const char* texture_filename);
  void clean();

  void draw(const RenderParams& render_params);
  const Shape* shape;

  GLuint vertex_array_id;

  std::vector<VertexAttribBuffer> attribute_buffers;
  IndexBuffer index_buffer;

  GLShader shader_attribs;

  GLuint texture;

  arma::fmat color;

 private:
  GLuint LoadTexture(const char* texture_filename);
};
}
