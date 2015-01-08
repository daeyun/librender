/**
 * @file mesh_loader.h
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright Scry is free software released under the BSD 2-Clause license.
 */
#pragma once

#include <armadillo>
#include "third_party/tinyobjloader/tiny_obj_loader.h"
#include "shape.h"
#include "graphics.h"

namespace scry {

void LoadObj(const RenderParams& render_params, Shape& mesh);
void ComputeNormals(const arma::fmat& v, const arma::umat& f, arma::fmat& vn);
void NormalizeCoords(arma::fmat& v);
void CrossCol(const arma::fmat& a, const arma::fmat& b, arma::fmat& c);
}
