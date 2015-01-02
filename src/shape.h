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

enum ShapeType { kTriangleMesh, kLines };

struct Shape {
  ShapeType type;

  arma::fmat v;
  arma::umat f;
  arma::fmat vn;
  arma::fmat uv;
  arma::fmat color;
};
}
