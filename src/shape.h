/**
 * @file shape.h
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright Scry is free software released under the BSD 2-Clause license.
 */
#pragma once

#include <armadillo>

namespace scry {
using arma::fmat;
using arma::umat;

enum class ShapeType { kTriangles, kLines, kPoints };

struct Shape {
  ShapeType type;

  fmat v;   // vertex position
  fmat vn;  // vertex normal
  fmat vc;  // vertex color
  fmat uv;  // vertex texture coordinate
  umat ind; // index
};
}
