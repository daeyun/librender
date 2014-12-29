#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "shader.h"
#include "gl_object.h"

class Annotation {
   public:
    Annotation(float grid_z);
    ~Annotation();
    void draw(const glm::mat4& projection, const glm::mat4& view,
              const glm::mat4& model);

    ScryObject* axis_data;
    ScryObject* grid_data;
    GLObject gl_axes;
    GLObject gl_grid;

    ShaderProperties shader_properties;
    
   private:
};
