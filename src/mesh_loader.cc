/**
 * @file mesh_loader.cc
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright Scry is free software released under the BSD 2-Clause license.
 */
#include "mesh_loader.h"

#include <stdexcept>
#include <armadillo>
#include <boost/format.hpp>
#include "config.h"
#include "graphics.h"
#include "shape.h"

namespace scry {

/**
 * @brief Import shape from a Wavefront .obj file. Vertex normals are estimated
 *        if not provided.
 * @param[in] render_params
 * @param[out] mesh
 */
void LoadObj(const RenderParams& render_params, Shape& mesh) {
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;

  std::string err = tinyobj::LoadObj(shapes, materials, render_params.in_filename.c_str());

  if (!err.empty()) throw std::runtime_error(err);
  if (shapes.empty())
    throw std::runtime_error(std::string("Shape not found in ") + render_params.in_filename);

  const int kVertexDims = 3;
  const int kTextureDims = 2;

  for (tinyobj::shape_t shape : shapes) {
    if (shape.mesh.positions.empty())
      throw std::runtime_error(std::string("Vertex not found in ") + render_params.in_filename);
    if (shape.mesh.indices.empty())
      throw std::runtime_error(std::string("Face not found in ") + render_params.in_filename);

    int kNumVertex = shape.mesh.positions.size() / kVertexDims;
    int kNumFace = shape.mesh.indices.size() / kVertexDims;
    arma::fmat shape_v(&shape.mesh.positions[0], kVertexDims, kNumVertex);
    arma::umat shape_f(&shape.mesh.indices[0], kVertexDims, kNumFace);
    arma::fmat shape_n;
    arma::fmat shape_uv;

    if (shape.mesh.normals.empty()) {
      ComputeNormals(shape_v, shape_f, shape_n);
    } else {
      shape_n = arma::fmat(&shape.mesh.normals[0], kVertexDims, kNumVertex);
    }

    if (!shape.mesh.texcoords.empty()) {
      shape_uv = arma::fmat(&shape.mesh.texcoords[0], kTextureDims, kNumVertex);
      shape_uv.swap_rows(0, 1);
    }

    mesh.v = join_cols(mesh.v, shape_v);
    mesh.f = join_cols(mesh.f, shape_f);
    mesh.vn = join_cols(mesh.vn, shape_n);
    mesh.uv = join_cols(mesh.uv, shape_uv);
  }

  mesh.vn = arma::normalise(mesh.vn);

  if (render_params.will_normalize) {
    NormalizeCoords(mesh.v);
  }

  switch (render_params.up_axis) {
    case X:
      mesh.v = arma::join_vert(mesh.v.rows(1, 2), mesh.v.row(0));
      mesh.vn = arma::join_vert(mesh.vn.rows(1, 2), mesh.vn.row(0));
      break;
    case Y:
      mesh.v = arma::join_vert(mesh.v.row(2), mesh.v.rows(0, 1));
      mesh.vn = arma::join_vert(mesh.vn.row(2), mesh.vn.rows(0, 1));
      break;
    case Z:
      // Nothing to do
      break;
  }

  // RGBA color per vertex
  mesh.color.reshape(4, mesh.v.n_cols);
  mesh.color.each_col() = render_params.color;

  mesh.type = kTriangleMesh;
}

/**
 * @brief Compute the face and vertex normals from a triangle mesh of indexed
 *        vertices.
 * @param v[in] 3 by n vertices
 * @param f[in] 3 by m faces
 * @param vn[out] 3 by n vertex normals
 */
void ComputeNormals(const arma::fmat& v, const arma::umat& f, arma::fmat& vn) {
  arma::fmat a = v.cols(f.row(1)) - v.cols(f.row(0));
  arma::fmat b = v.cols(f.row(2)) - v.cols(f.row(0));

  // Cross product of each pair of columns
  arma::fmat fn;
  CrossCol(a, b, fn);

  // Accumulate vertex normal
  vn.zeros(v.n_rows, v.n_cols);
  arma::umat indices = reshape(f.t(), 1, f.n_cols * 3);
  vn.cols(indices) += arma::repmat(fn, 1, 3);

  arma::fmat num_adj_f;
  num_adj_f.zeros(1, v.n_cols);
  num_adj_f(indices) += 1;

  // Divide by the number of adjacent faces
  vn /= arma::repmat(num_adj_f, 3, 1);
}

/**
 * @brief Compute a column-wise cross product of \a a and \a b.
 * @param[in] a,b 3 by m matrices
 * @param[out] out 3 by m matrix
 */
void CrossCol(const arma::fmat& a, const arma::fmat& b, arma::fmat& out) {
  out = join_cols(join_cols(a.row(1) % b.row(2) - a.row(2) % b.row(1),
                            a.row(2) % b.row(0) - a.row(0) % b.row(2)),
                  a.row(0) % b.row(1) - a.row(1) % b.row(0));
}

/**
 * @brief Re-scale the data points so that the maximum dimension range is 1 and
 *        the center of the object is at (0, 0).
 * @param v 3 by n matrix of vertices.
 */
void NormalizeCoords(arma::fmat& v) {
  arma::fvec vmax = arma::max(v, 1);
  arma::fvec vmin = arma::min(v, 1);

  arma::fvec range = vmax - vmin;

  v.each_col() -= vmin + range / 2;
  v /= arma::max(range);
}
}
