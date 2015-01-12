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
#include <unordered_map>
#include "shape.h"
#include "databuffer.h"
#include "shader.h"
#include "graphics.h"

namespace scry {
namespace shader {
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;
using std::string;
using std::unordered_map;
using std::vector;

enum class AttribType { kMat4, kMat3, kInt, kFloat, kVec3, kVec4 };

struct ShaderMatrices {
  GLuint model_view;
  GLuint projection;
  GLuint model_view_projection;
  GLuint vector_model_view;
};

struct ShaderAttribute {
  ShaderAttribute() = default;
  ShaderAttribute(AttribType type, float data) : type(type), data_float(data){};
  ShaderAttribute(AttribType type, int data) : type(type), data_int(data){};
  ShaderAttribute(AttribType type, void* data)
      : type(type), data_pointer(data){};

  AttribType type;
  GLuint uniform_location;
  void* data_pointer;
  int data_int;
  float data_float;
};

enum DataBufferLocation {
  kVertex = 0,
  kVertexNormal = 1,
  kVertexColor = 2,
  kVertexTexCoord = 3
};

class ShaderObject {
 public:
  ShaderObject() = default;
  ShaderObject(const Shape* shape, const GLuint shader_id,
               const RenderParams& render_params);
  virtual ~ShaderObject();

  virtual void Draw(const RenderParams& render_params,
                    bool is_view_space = true);

  GLuint vertex_array_id;
  GLuint shader_id;

  const Shape* shape;

  VertexAttribBuffer* position_buffer = nullptr;
  VertexAttribBuffer* normal_buffer = nullptr;
  VertexAttribBuffer* color_buffer = nullptr;
  VertexAttribBuffer* texture_buffer = nullptr;
  IndexBuffer* index_buffer = nullptr;

  ShaderMatrices matrices;
  unordered_map<string, ShaderAttribute> attribs;

 protected:
  void SetupVAO(const Shape* shape);
  void UpdateVAO(const glm::mat4& mv, const glm::mat3& normal_mat);
  void FillUniformLocations(const GLuint shader_id,
                            unordered_map<string, ShaderAttribute>& attribs);

  const size_t kNumMaxLights = 20;
  vector<vec3> light_positions {kNumMaxLights};

 private:
  virtual void SetAttributes(
      const RenderParams& render_params,
      unordered_map<string, ShaderAttribute>& attribs) = 0;
};
}
}
