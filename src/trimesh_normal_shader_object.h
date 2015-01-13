#pragma once

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "shape.h"
#include "databuffer.h"
#include "shader.h"
#include "shaders/trimesh_normal_shader.h"
#include "shader_object.h"

namespace librender {
namespace shader {
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;
using std::vector;

class TrimeshNormalShaderObject : public ShaderObject {
 public:
  TrimeshNormalShaderObject() = default;
  TrimeshNormalShaderObject(const Shape* shape, const GLuint shader_id,
                            const RenderParams& render_params);

 private:
  void SetAttributes(const RenderParams& render_params,
                     unordered_map<string, ShaderAttribute>& attribs) override;
};
}
}
