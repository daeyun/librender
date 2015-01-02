/**
 * @file annotation.cc
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright Scry is free software released under the BSD 2-Clause license.
 */
#include "annotation.h"

#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "shaders/simple_shader.h"
#include "shape.h"
#include "shader_object.h"
#include "config.h"

extern GLuint scry_current_shader;

namespace scry {

Annotation::Annotation(float grid_z) {
  std::vector<std::string> labels = {"x", "y", "z"};
  float length = 0.6;
  arma::fmat origin = {0, 0, 0};
  arma::fmat marking_range = {0, 1};

  GLuint simple_shader = shader::Shader(kSimpleShader);

  if (config::are_axes_visible) {
    this->axis_data = new Shape();
    this->axis_data->type = kLines;
    this->axis_data->v.reshape(3, 6);
    this->axis_data->f.reshape(2, 3);
    this->axis_data->color.reshape(4, 6);
    this->axis_data->color.each_col() = arma::fvec4({0.5, 0.5, 0.5, 1});

    for (size_t i = 0; i < labels.size(); i++) {
      arma::fmat start = origin;
      arma::fmat end = origin;
      start(i) -= length;
      end(i) += length;

      this->axis_data->v.col(2 * i) = start.t();
      this->axis_data->v.col(2 * i + 1) = end.t();
      this->axis_data->f(0, i) = 2 * i;
      this->axis_data->f(1, i) = 2 * i + 1;
    }

    this->gl_axes = ShaderObject(this->axis_data, simple_shader, NULL);
  }

  length = 1.5;
  float spacing = 0.1;

  this->grid_data = new Shape();
  this->grid_data->type = kLines;
  this->grid_data->v.reshape(3, int(2 * length / spacing + 1) * 4);
  this->grid_data->f.reshape(2, int(2 * length / spacing + 1) * 2);
  this->grid_data->color.reshape(4, int(2 * length / spacing + 1) * 4);
  this->grid_data->color.each_col() = arma::fvec4({0.8, 0.8, 0.8, 1});

  size_t vcount = 0;
  size_t lcount = 0;
  for (float x = -length; x <= length + spacing; x += spacing) {
    arma::fvec start = {x, -length, grid_z};
    arma::fvec end = {x, length, grid_z};

    this->grid_data->v.col(vcount) = start;
    this->grid_data->f(0, lcount) = vcount;
    vcount++;

    this->grid_data->v.col(vcount) = end;
    this->grid_data->f(1, lcount) = vcount;
    vcount++;
    lcount++;
  }

  for (float x = -length; x <= length + spacing; x += spacing) {
    arma::fvec start = {-length, x, grid_z};
    arma::fvec end = {length, x, grid_z};

    this->grid_data->v.col(vcount) = start;
    this->grid_data->f(0, lcount) = vcount;
    vcount++;

    this->grid_data->v.col(vcount) = end;
    this->grid_data->f(1, lcount) = vcount;
    vcount++;
    lcount++;
  }

  this->gl_grid = ShaderObject(this->grid_data, simple_shader, NULL);
}

Annotation::~Annotation() {
  if (config::are_axes_visible) {
    delete this->axis_data;
    this->gl_axes.clean();
  }
  delete this->grid_data;
  this->gl_grid.clean();
}

void Annotation::draw(const glm::mat4& projection, const glm::mat4& view,
                      const glm::mat4& model) {
  shader_properties.mv = view * model;
  shader_properties.mvp = projection * shader_properties.mv;

  if (config::are_axes_visible) {
    gl_axes.draw(shader_properties);
  }
  gl_grid.draw(shader_properties);
}
}
