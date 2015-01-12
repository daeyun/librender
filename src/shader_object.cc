/**
 * @file shader_object.cc
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright Scry is free software released under the BSD 2-Clause license.
 */
#include "shader_object.h"

#include <exception>
#include <armadillo>
#include <vector>
#include <boost/algorithm/string.hpp>
#include "shader.h"
#include "graphics.h"
#include "databuffer.h"
#include "matrix_util.h"
#include "debug.h"

namespace scry {
namespace shader {

ShaderObject::ShaderObject(const Shape* shape, const GLuint shader_id,
                           const RenderParams& render_params) {
  glGenVertexArrays(1, &this->vertex_array_id);
  glBindVertexArray(this->vertex_array_id);

  this->shader_id = shader_id;
  this->shape = shape;

  SetAttributes(render_params, this->attribs);
  FillUniformLocations(shader_id, this->attribs);
  SetupVAO(shape);
}

/**
 * @brief Copy data to data buffers and attach them to the vertex array object.
 */
void ShaderObject::SetupVAO(const Shape* shape) {
  glBindVertexArray(vertex_array_id);

  position_buffer = new VertexAttribBuffer(
      GL_ARRAY_BUFFER, DataBufferLocation::kVertex, shape->v.n_rows, GL_FLOAT,
      &shape->v[0], shape->v.n_elem * sizeof(float));

  if (!shape->vn.empty()) {
    normal_buffer = new VertexAttribBuffer(
        GL_ARRAY_BUFFER, DataBufferLocation::kVertexNormal, shape->vn.n_rows,
        GL_FLOAT, &shape->vn[0], shape->vn.n_elem * sizeof(float));
  }

  if (!shape->vc.empty()) {
    color_buffer = new VertexAttribBuffer(
        GL_ARRAY_BUFFER, DataBufferLocation::kVertexColor, shape->vc.n_rows,
        GL_FLOAT, &shape->vc[0], shape->vc.n_elem * sizeof(float));
  }

  if (!shape->uv.empty()) {
    texture_buffer = new VertexAttribBuffer(
        GL_ARRAY_BUFFER, DataBufferLocation::kVertexTexCoord, shape->uv.n_rows,
        GL_FLOAT, &shape->uv[0], shape->uv.n_elem * sizeof(float));
  }

  GLenum draw_mode;
  switch (shape->type) {
    case ShapeType::kTriangles:
      draw_mode = GL_TRIANGLES;
      break;
    case ShapeType::kLines:
      draw_mode = GL_LINES;
      break;
    case ShapeType::kPoints:
      draw_mode = GL_POINTS;
      break;
  }

  index_buffer = new IndexBuffer(
      GL_ELEMENT_ARRAY_BUFFER, draw_mode, shape->ind.n_elem, GL_UNSIGNED_INT,
      &shape->ind[0], shape->ind.n_elem * sizeof(arma::uword));
}

/**
 * @brief Updates the vertex positions and normals.
 */
void ShaderObject::UpdateVAO(const glm::mat4& mv, const glm::mat3& normal_mat) {
  if (!shape->v.empty()) {
    auto vertex_view = scry::util::MatMult(mv, shape->v);
    void* data_ptr = vertex_view.memptr();
    size_t data_size = vertex_view.n_elem * sizeof(vertex_view[0]);
    position_buffer->SetBufferData(data_ptr, data_size);
  }

  if (!shape->vn.empty()) {
    auto vertex_view = scry::util::MatMult(normal_mat, shape->v);
    void* data_ptr = vertex_view.memptr();
    size_t data_size = vertex_view.n_elem * sizeof(vertex_view[0]);
    normal_buffer->SetBufferData(data_ptr, data_size);
  }
}

/**
 * @brief
 * @param[in] shader_id
 * @param[in,out] shader_globals
 */
void ShaderObject::FillUniformLocations(
    GLuint shader_id,
    std::unordered_map<string, ShaderAttribute>& shader_globals) {
  _assert(glIsProgram(shader_id));

  this->matrices.model_view_projection =
      glGetUniformLocation(shader_id, "iModelViewProjectionMatrix");
  this->matrices.model_view =
      glGetUniformLocation(shader_id, "iModelViewMatrix");
  this->matrices.vector_model_view =
      glGetUniformLocation(shader_id, "iVectorModelViewMatrix");
  this->matrices.projection =
      glGetUniformLocation(shader_id, "iProjectionMatrix");

  for (auto& kv : shader_globals) {
    kv.second.uniform_location =
        glGetUniformLocation(shader_id, kv.first.c_str());
  }
}

ShaderObject::~ShaderObject() {
  _var(position_buffer);

  if (position_buffer) {
    delete position_buffer;
  }
  if (normal_buffer) {
    delete normal_buffer;
  }
  if (color_buffer) {
    delete color_buffer;
  }
  if (index_buffer) {
    delete index_buffer;
  }

  glDeleteVertexArrays(1, &this->vertex_array_id);
}

void ShaderObject::Draw(const RenderParams& render_params, bool is_view_space) {
  glBindVertexArray(this->vertex_array_id);
  glUseProgram(this->shader_id);

  GLint is_linked;
  int log_length;
  glGetProgramiv(this->shader_id, GL_LINK_STATUS, &is_linked);
  glGetProgramiv(this->shader_id, GL_INFO_LOG_LENGTH, &log_length);
  if (log_length > 0 || !is_linked) {
    std::vector<char> error_message(log_length + 1);
    glGetProgramInfoLog(this->shader_id, log_length, nullptr,
                        &error_message[0]);
    throw std::runtime_error(&error_message[0]);
  }

  mat4 mv = render_params.shader_params.view_mat *
            render_params.shader_params.model_mat;
  mat4 mvp = render_params.shader_params.projection_mat * mv;
  // inverse transpose of the upper left 3x3 submatrix of the modelview matrix
  mat3 normal_mat = mat3(glm::transpose(glm::inverse(mv)));

  if (is_view_space) {
    UpdateVAO(mv, normal_mat);
    for (size_t i = 0; i < render_params.shader_params.lights.size(); ++i) {
      this->light_positions[i] = util::MatMult(
          mv, render_params.shader_params.lights[i].light_position);
      _var(this->light_positions[i][0]);
      _var(this->light_positions[i][1]);
      _var(this->light_positions[i][2]);
    }
  }

  glUniformMatrix4fv(this->matrices.model_view_projection, 1, GL_FALSE,
                     &mvp[0][0]);
  glUniformMatrix4fv(this->matrices.model_view, 1, GL_FALSE, &mv[0][0]);
  glUniformMatrix4fv(this->matrices.projection, 1, GL_FALSE,
                     &render_params.shader_params.projection_mat[0][0]);
  glUniformMatrix3fv(this->matrices.vector_model_view, 1, GL_FALSE,
                     &normal_mat[0][0]);

  for (auto& kv : this->attribs) {
    switch (kv.second.type) {
      case AttribType::kInt:
        glUniform1i(kv.second.uniform_location, kv.second.data_int);
        break;
      case AttribType::kFloat:
        glUniform1f(kv.second.uniform_location, kv.second.data_float);
        break;
      case AttribType::kVec3:
        glUniform3fv(kv.second.uniform_location, 1,
                     (const float*)kv.second.data_pointer);
        break;
      default:
        throw std::runtime_error("Unreconized shader attribute type");
        break;
    }
  }

  this->index_buffer->draw();
}
}
}
