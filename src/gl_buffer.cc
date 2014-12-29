#include "gl_buffer.h"

GLBuffer::GLBuffer(GLenum target, GLenum gl_type, const GLvoid* data,
                   GLsizeiptr data_bytes)
    : is_initialized(false),
      buffer_id(0),
      target(target),
      gl_type(gl_type),
      data(data),
      data_bytes((GLsizeiptr)data_bytes) {
    glGenBuffers(1, &buffer_id);
    glBindBuffer(target, buffer_id);
    glBufferData(target, data_bytes, data, GL_STATIC_DRAW);
    is_initialized = true;
}

void GLBuffer::clean() { glDeleteBuffers(1, &this->buffer_id); }

GLVertexAttribBuffer::GLVertexAttribBuffer(GLenum target, GLuint attrib_index,
                                           GLint attrib_size, GLenum gl_type,
                                           const GLvoid* data,
                                           size_t data_bytes)
    : GLBuffer(target, gl_type, data, data_bytes),
      attrib_index(attrib_index),
      attrib_size(attrib_size) {
    glEnableVertexAttribArray(attrib_index);
    glVertexAttribPointer(attrib_index,  // attribute
                          attrib_size,   // size
                          gl_type,       // type
                          GL_FALSE,      // normalized?
                          0,             // stride
                          (void*)0       // array buffer offset
                          );
};

GLIndexBuffer::GLIndexBuffer(GLenum target, GLenum mode, GLint num_item,
                             GLenum gl_type, const GLvoid* data,
                             size_t data_bytes)
    : GLBuffer(target, gl_type, data, data_bytes),
      mode(mode),
      num_item(num_item){};

void GLIndexBuffer::draw() {
    glBindBuffer(target, buffer_id);
    glDrawElements(mode,      // mode
                   num_item,  // count
                   gl_type,   // type
                   (void*)0   // element array buffer offset
                   );
}
