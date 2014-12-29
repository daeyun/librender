#pragma once

#include <armadillo>
#include "tiny_obj_loader.h"
#include "scry_object.h"

void load_obj(const std::string obj_file, ScryObject& mesh);
void compute_normals(const arma::fmat& v, const arma::umat& f, arma::fmat& vn);
void normalize_coords(arma::fmat& v);
void cross_col(const arma::fmat& a, const arma::fmat& b, arma::fmat& c);
