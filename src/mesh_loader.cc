#include "mesh_loader.h"

#include <stdexcept>
#include <armadillo>
#include <boost/format.hpp>
#include "config.h"
#include "scry_object.h"

void load_obj(const std::string obj_file, ScryObject& mesh) {
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err = tinyobj::LoadObj(shapes, materials, obj_file.c_str());

    if (!err.empty())
        throw std::runtime_error(err);
    if (shapes.empty())
        throw std::runtime_error(std::string("Shape not found in ") + obj_file);

    const int kVertexDims = 3;
    const int kTextureDims = 2;

    for (tinyobj::shape_t shape : shapes) {
        if (shape.mesh.positions.empty())
            throw std::runtime_error(std::string("Vertex not found in ") + obj_file);
        if (shape.mesh.indices.empty())
            throw std::runtime_error(std::string("Face not found in ") + obj_file);

        int kNumVertex = shape.mesh.positions.size() / kVertexDims;
        int kNumFace = shape.mesh.indices.size() / kVertexDims;
        arma::fmat shape_v(&shape.mesh.positions[0], kVertexDims, kNumVertex);
        arma::umat shape_f(&shape.mesh.indices[0], kVertexDims, kNumFace);
        arma::fmat shape_n;
        arma::fmat shape_uv;

        if (shape.mesh.normals.empty()) {
            compute_normals(shape_v, shape_f, shape_n);
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

    if (config::will_normalize) {
        normalize_coords(mesh.v);
    }

    switch (config::up_axis) {
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

    mesh.cdata.reshape(4, mesh.v.n_cols);
    mesh.cdata.each_col() = config::color;
    
    mesh.type = kTriangleMesh;
}

void compute_normals(const arma::fmat& v, const arma::umat& f, arma::fmat& vn) {
    arma::fmat a = v.cols(f.row(1)) - v.cols(f.row(0));
    arma::fmat b = v.cols(f.row(2)) - v.cols(f.row(0));

    // Cross product of each pair of columns
    arma::fmat fn;
    cross_col(a, b, fn);

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

/* Cross product of each pair of columns */
void cross_col(const arma::fmat& a, const arma::fmat& b, arma::fmat& c) {
    c = join_cols(join_cols(a.row(1) % b.row(2) - a.row(2) % b.row(1),
                            a.row(2) % b.row(0) - a.row(0) % b.row(2)),
                            a.row(0) % b.row(1) - a.row(1) % b.row(0));
}

void normalize_coords(arma::fmat& v) {
    arma::fvec vmax = arma::max(v, 1);
    arma::fvec vmin = arma::min(v, 1);

    arma::fvec range = vmax - vmin;

    v.each_col() -= vmin + range / 2;
    v /= arma::max(range);
}
