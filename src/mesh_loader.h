#ifndef MESH_LOADER_H_
#define MESH_LOADER_H_

#include <armadillo>
#include "thirdparty/tiny_obj_loader.h"
#include "mesh.h"

void load_obj(const std::string obj_file, Mesh& mesh);
void compute_normals(const arma::fmat& v, const arma::umat& f, arma::fmat& vn);
void normalize_coords(arma::fmat& v);

#endif  // MESH_LOADER_H_
