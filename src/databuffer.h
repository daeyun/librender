/**
 * @file databuffer.h
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright librender is free software released under the BSD 2-Clause
 * license.
 */
#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace librender {

class DataBuffer {
 public:
  DataBuffer() : is_initialized(false){};
  ~DataBuffer();
  DataBuffer(GLenum target, GLenum gl_type, const void* data,
             GLsizeiptr data_bytes, bool is_static = true);
  void SetBufferData(const void* data, const size_t size,
                     bool is_static = true);

  bool is_initialized;
  GLuint buffer_id;
  GLenum target;
  GLenum gl_type;
};

class VertexAttribBuffer : public DataBuffer {
 public:
  VertexAttribBuffer();
  VertexAttribBuffer(GLenum target, GLuint attrib_index, GLint attrib_size,
                     GLenum gl_type, const void* data, size_t data_bytes,
                     bool is_static = true);
  GLuint attrib_index;
  GLint attrib_size;
};

class IndexBuffer : public DataBuffer {
 public:
  IndexBuffer() : DataBuffer(){};
  IndexBuffer(GLenum target, GLenum mode, GLint num_item, GLenum gl_type,
              const void* data, size_t data_bytes, bool is_static = true);
  void draw();

  GLenum mode;
  GLint num_item;
};
}
