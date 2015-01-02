/**
 * @file databuffer.h
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright Scry is free software released under the BSD 2-Clause license.
 */
#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace scry {

class DataBuffer {
 public:
  DataBuffer() : is_initialized(false){};
  DataBuffer(GLenum target, GLenum gl_type, const GLvoid* data,
             GLsizeiptr data_bytes);
  void clean();

  bool is_initialized;
  GLuint buffer_id;
  GLenum target;
  GLenum gl_type;
  const GLvoid* data;
  GLsizeiptr data_bytes;
};

class VertexAttribBuffer : public DataBuffer {
 public:
  VertexAttribBuffer();
  VertexAttribBuffer(GLenum target, GLuint attrib_index, GLint attrib_size,
                     GLenum gl_type, const GLvoid* data, size_t data_bytes);
  GLuint attrib_index;
  GLint attrib_size;
};

class IndexBuffer : public DataBuffer {
 public:
  IndexBuffer() : DataBuffer(){};
  IndexBuffer(GLenum target, GLenum mode, GLint num_item, GLenum gl_type,
              const GLvoid* data, size_t data_bytes);
  void draw();

  GLenum mode;
  GLint num_item;
};
}
