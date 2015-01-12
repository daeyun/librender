/**
 * @file annotation.cc
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright Scry is free software released under the BSD 2-Clause license.
 */
#include "annotation.h"
#include "debug.h"

#include "line_shader_object.h"

namespace scry {

Annotation::Annotation(float grid_z, RenderParams& render_params) {
  std::vector<std::string> labels = {"x", "y", "z"};
  float length = 0.6;
  arma::fmat origin = {0, 0, 0};
  arma::fmat marking_range = {0, 1};

  this->line_shader = shader::Shader(shader::kLineShader);

  if (render_params.are_axes_visible) {
    this->axis_data = new Shape();
    this->axis_data->type = ShapeType::kLines;
    this->axis_data->v.reshape(3, 6);
    this->axis_data->ind.reshape(2, 3);
    this->axis_data->vc.reshape(4, 6);
    this->axis_data->vc.each_col() = arma::fvec4({0.5, 0.5, 0.5, 1});

    for (size_t i = 0; i < labels.size(); i++) {
      arma::fmat start = origin;
      arma::fmat end = origin;
      start(i) -= length;
      end(i) += length;

      this->axis_data->v.col(2 * i) = start.t();
      this->axis_data->v.col(2 * i + 1) = end.t();
      this->axis_data->ind(0, i) = 2 * i;
      this->axis_data->ind(1, i) = 2 * i + 1;
    }

    this->gl_axes = new shader::LineShaderObject(this->axis_data, line_shader, render_params);
  } else {
    this->axis_data = nullptr;
  }

  length = 1.5;
  float spacing = 0.1;

  this->grid_data = new Shape();
  this->grid_data->type = ShapeType::kLines;
  this->grid_data->v.reshape(3, int(2 * length / spacing + 1) * 4);
  this->grid_data->ind.reshape(2, int(2 * length / spacing + 1) * 2);
  this->grid_data->vc.reshape(4, int(2 * length / spacing + 1) * 4);
  this->grid_data->vc.each_col() = arma::fvec4({0.8, 0.8, 0.8, 1});

  size_t vcount = 0;
  size_t lcount = 0;
  for (float x = -length; x <= length + spacing; x += spacing) {
    arma::fvec start = {x, -length, grid_z};
    arma::fvec end = {x, length, grid_z};

    this->grid_data->v.col(vcount) = start;
    this->grid_data->ind(0, lcount) = vcount;
    vcount++;

    this->grid_data->v.col(vcount) = end;
    this->grid_data->ind(1, lcount) = vcount;
    vcount++;
    lcount++;
  }

  for (float x = -length; x <= length + spacing; x += spacing) {
    arma::fvec start = {-length, x, grid_z};
    arma::fvec end = {length, x, grid_z};

    this->grid_data->v.col(vcount) = start;
    this->grid_data->ind(0, lcount) = vcount;
    vcount++;

    this->grid_data->v.col(vcount) = end;
    this->grid_data->ind(1, lcount) = vcount;
    vcount++;
    lcount++;
  }

  this->gl_grid = new shader::LineShaderObject(this->grid_data, line_shader, render_params);
}

Annotation::~Annotation() {
  if (this->axis_data != nullptr) {
    delete this->axis_data;
    delete this->gl_axes;
  }

  if (this->grid_data != nullptr) {
    delete this->grid_data;
    delete this->gl_grid;
  }
}

void Annotation::Draw(const RenderParams& render_params) {
  if (render_params.are_axes_visible) {
    gl_axes->Draw(render_params, true);
  }
  gl_grid->Draw(render_params, true);
}
}
