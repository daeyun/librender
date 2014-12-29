#pragma once

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "scry_object.h"
#include "gl_buffer.h"
#include "shader.h"

class GLObject {
   public:
    GLObject();
    GLObject(const ScryObject* scry_object, const GLuint shader_id,
             const char* texture_filename);
    void clean();

    void draw(const ShaderProperties& shader_properties);
    const ScryObject* scry_object;

    GLuint vertex_array_id;

    std::vector<GLVertexAttribBuffer> attribute_buffers;
    GLIndexBuffer index_buffer;

    GLShader shader_attribs;

    GLuint texture;

    arma::fmat cdata;

   private:
    GLuint load_texture(const char* texture_filename);
};
