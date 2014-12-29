#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class GLBuffer {
   public:
    GLBuffer() : is_initialized(false){};
    GLBuffer(GLenum target, GLenum gl_type, const GLvoid* data,
             GLsizeiptr data_bytes);
    void clean();

    bool is_initialized;
    GLuint buffer_id;
    GLenum target;
    GLenum gl_type;
    const GLvoid* data;
    GLsizeiptr data_bytes;
};

class GLVertexAttribBuffer : public GLBuffer {
   public:
    GLVertexAttribBuffer();
    GLVertexAttribBuffer(GLenum target, GLuint attrib_index, GLint attrib_size,
                         GLenum gl_type, const GLvoid* data, size_t data_bytes);
    GLuint attrib_index;
    GLint attrib_size;
};

class GLIndexBuffer : public GLBuffer {
   public:
    GLIndexBuffer() : GLBuffer(){};
    GLIndexBuffer(GLenum target, GLenum mode, GLint num_item, GLenum gl_type,
                  const GLvoid* data, size_t data_bytes);
    void draw();

    GLenum mode;
    GLint num_item;
};
