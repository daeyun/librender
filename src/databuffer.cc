/**
 * @file databuffer.cc
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright Scry is free software released under the BSD 2-Clause license.
 */
#include "databuffer.h"

namespace scry {

DataBuffer::DataBuffer(GLenum target, GLenum gl_type, const void* data,
                       GLsizeiptr data_bytes, bool is_static)
    : is_initialized(false), buffer_id(0), target(target), gl_type(gl_type) {
  glGenBuffers(1, &this->buffer_id);
  SetBufferData(data, data_bytes, is_static);
  is_initialized = true;
}

DataBuffer::~DataBuffer() { glDeleteBuffers(1, &this->buffer_id); }

void DataBuffer::SetBufferData(const void* data, const size_t size,
                               bool is_static) {
  // Bind buffer_id to a global variable
  glBindBuffer(this->target, this->buffer_id);
  auto hint = is_static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
  // Copy data to current buffer
  glBufferData(this->target, size, data, hint);
}

VertexAttribBuffer::VertexAttribBuffer(GLenum target, GLuint attrib_index,
                                       GLint attrib_size, GLenum gl_type,
                                       const void* data, size_t data_bytes,
                                       bool is_static)
    : DataBuffer(target, gl_type, data, data_bytes, is_static),
      attrib_index(attrib_index),
      attrib_size(attrib_size) {
  glBindBuffer(this->target, this->buffer_id);
  glEnableVertexAttribArray(attrib_index);
  // This tells the VAO about the current buffer_id
  glVertexAttribPointer(attrib_index,  // attribute
                        attrib_size,   // size
                        gl_type,       // type
                        GL_FALSE,      // normalized?
                        0,             // stride
                        (void*)0       // array buffer offset
                        );
};

IndexBuffer::IndexBuffer(GLenum target, GLenum mode, GLint num_item,
                         GLenum gl_type, const void* data, size_t data_bytes,
                         bool is_static)
    : DataBuffer(target, gl_type, data, data_bytes, is_static),
      mode(mode),
      num_item(num_item){};

void IndexBuffer::draw() {
  glBindBuffer(this->target, this->buffer_id);
  glDrawElements(mode,      // mode
                 num_item,  // count
                 gl_type,   // type
                 (void*)0   // element array buffer offset
                 );
}
}
