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

namespace scry {

ShaderObject::ShaderObject() : texture(0) {}

ShaderObject::ShaderObject(const Shape* shape, const GLuint shader_id,
                           const char* texture_filename)
    : texture(0) {
  glGenVertexArrays(1, &this->vertex_array_id);
  glBindVertexArray(this->vertex_array_id);

  if (texture_filename != nullptr) {
    // TODO
    this->texture = LoadTexture(texture_filename);
  }

  this->shader_attribs.id = shader_id;
  this->shader_attribs.mvp = glGetUniformLocation(shader_id, "MVPMatrix");
  this->shader_attribs.mv = glGetUniformLocation(shader_id, "MVMatrix");
  this->shader_attribs.normal_mat =
      glGetUniformLocation(shader_id, "NormalMatrix");

  this->shader_attribs.num_lights =
      glGetUniformLocation(shader_id, "NumLights");
  this->shader_attribs.eye_direction =
      glGetUniformLocation(shader_id, "EyeDirection");
  this->shader_attribs.shininess = glGetUniformLocation(shader_id, "Shininess");
  this->shader_attribs.strength = glGetUniformLocation(shader_id, "Strength");
  this->shader_attribs.ambient = glGetUniformLocation(shader_id, "Ambient");

  for (size_t i = 0; i < kNumMaxLights; i++) {
    std::string name_prefix = "Lights";
    name_prefix += "[" + std::to_string(i) + "].";

    GLShaderLight light;
    light.isEnabled =
        glGetUniformLocation(shader_id, (name_prefix + "isEnabled").c_str());
    light.LightColor =
        glGetUniformLocation(shader_id, (name_prefix + "LightColor").c_str());
    light.LightPosition = glGetUniformLocation(
        shader_id, (name_prefix + "LightPosition").c_str());
    light.ConstantAttenuation = glGetUniformLocation(
        shader_id, (name_prefix + "ConstantAttenuation").c_str());
    light.LinearAttenuation = glGetUniformLocation(
        shader_id, (name_prefix + "LinearAttenuation").c_str());
    light.QuadraticAttenuation = glGetUniformLocation(
        shader_id, (name_prefix + "QuadraticAttenuation").c_str());

    this->shader_attribs.lights.push_back(light);
  }

  this->shape = shape;

  attribute_buffers.push_back(VertexAttribBuffer(
      GL_ARRAY_BUFFER, 0, this->shape->v.n_rows, GL_FLOAT, &this->shape->v[0],
      this->shape->v.n_elem * sizeof(float)));

  if (!this->shape->vn.empty()) {
    attribute_buffers.push_back(VertexAttribBuffer(
        GL_ARRAY_BUFFER, 1, this->shape->vn.n_rows, GL_FLOAT,
        &this->shape->vn[0], this->shape->vn.n_elem * sizeof(float)));
  }

  if (!this->shape->color.empty()) {
    attribute_buffers.push_back(VertexAttribBuffer(
        GL_ARRAY_BUFFER, 2, this->shape->color.n_rows, GL_FLOAT,
        &this->shape->color[0], this->shape->color.n_elem * sizeof(float)));
  }

  if (texture_filename != nullptr) {
    attribute_buffers.push_back(VertexAttribBuffer(
        GL_ARRAY_BUFFER, 3, this->shape->uv.n_rows, GL_FLOAT,
        &this->shape->uv[0], this->shape->uv.n_elem * sizeof(float)));
  }

  GLenum draw_mode = GL_TRIANGLES;
  if (shape->type == kLines) {
    draw_mode = GL_LINES;
  }

  index_buffer =
      IndexBuffer(GL_ELEMENT_ARRAY_BUFFER, draw_mode, this->shape->f.n_elem,
                  GL_UNSIGNED_INT, &this->shape->f[0],
                  this->shape->f.n_elem * sizeof(arma::uword));
}

void ShaderObject::clean() {
  for (VertexAttribBuffer buf : this->attribute_buffers) {
    buf.clean();
  }

  glDeleteVertexArrays(1, &this->vertex_array_id);
  if (this->texture != 0) {
    glDeleteTextures(1, &this->texture);
  }
}

GLuint ShaderObject::LoadTexture(const char* texture_filename) {
  std::string filename_str(texture_filename);
  std::string ext = filename_str.substr(filename_str.find_last_of(".") + 1);

  // TODO
  throw std::runtime_error("Not implemented");
}

void ShaderObject::draw(const RenderParams& render_params) {
  auto mv = render_params.shader_params.view_mat *
            render_params.shader_params.model_mat;
  auto mvp = render_params.shader_params.projection_mat * mv;
  auto normal_mat = glm::transpose(glm::inverse(mv));

  glBindVertexArray(this->vertex_array_id);

  GLuint shader_id = this->shader_attribs.id;
  glUseProgram(shader_id);

  glUniformMatrix4fv(this->shader_attribs.mvp, 1, GL_FALSE, &mvp[0][0]);
  glUniformMatrix4fv(this->shader_attribs.mv, 1, GL_FALSE, &mv[0][0]);
  glUniformMatrix4fv(this->shader_attribs.normal_mat, 1, GL_FALSE,
                     &normal_mat[0][0]);
  glUniform1i(this->shader_attribs.num_lights,
              render_params.shader_params.lights.size());
  glUniform3fv(this->shader_attribs.eye_direction, 1,
               &render_params.shader_params.eye_direction[0]);
  glUniform1f(this->shader_attribs.shininess,
              render_params.shader_params.shininess);
  glUniform1f(this->shader_attribs.strength,
              render_params.shader_params.strength);
  glUniform3fv(this->shader_attribs.ambient, 1,
               &render_params.shader_params.ambient[0]);

  for (size_t i = 0; i < render_params.shader_params.lights.size(); i++) {
    glUniform1i(this->shader_attribs.lights[i].isEnabled,
                render_params.shader_params.lights[i].is_enabled);
    glUniform3fv(this->shader_attribs.lights[i].LightColor, 1,
                 &render_params.shader_params.lights[i].light_color[0]);
    glUniform3fv(this->shader_attribs.lights[i].LightPosition, 1,
                 &render_params.shader_params.lights[i].light_position[0]);
    glUniform1f(this->shader_attribs.lights[i].ConstantAttenuation,
                render_params.shader_params.lights[i].constant_attenuation);
    glUniform1f(this->shader_attribs.lights[i].LinearAttenuation,
                render_params.shader_params.lights[i].linear_attenuation);
    glUniform1f(this->shader_attribs.lights[i].QuadraticAttenuation,
                render_params.shader_params.lights[i].quadratic_attenuation);
  }

  /* TODO
  if (this->texture != 0) {
      // Bind texture in Texture Unit 0
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, this->texture);
      glUniform1i(this->shader_attribs.texture, 0);
  } */

  this->index_buffer.draw();
}
}
