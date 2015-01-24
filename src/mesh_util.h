/**
 * @file mesh_util.h
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-10
 * @copyright librender is free software released under the BSD 2-Clause
 * license.
 */
#pragma once

#include <armadillo>
#include <vector>
#include <glm/glm.hpp>
#include <boost/coroutine/all.hpp>
#include <forward_list>
#include <initializer_list>
#include <queue>
#include <set>
#include <Eigen/Dense>
#include <igl/slice.h>
#include <numeric>
#include <algorithm>
#include <random>
#include <climits>
#include <utility>
#include "debug.h"

namespace librender {
namespace util {

template <typename T>
using coro = boost::coroutines::asymmetric_coroutine<T>;
using namespace Eigen;
using std::vector;
using std::forward_list;
using std::set;

struct Adjacency {
  vector<set<int>> v_to_f;
  vector<set<int>> v_to_v;
  vector<set<int>> f_to_f;
};

void FindConnectedComponents(const Adjacency& adj, vector<vector<int>>& result);
void FindOneFaceNormal(const MatrixX3d& vertex, const MatrixX3i& face,
                       int& face_index, RowVector3d& normal,
                       Matrix<double, 1, 6>& line);
void ComputeAdjacency(const MatrixX3i& face, size_t numv, Adjacency& adj);
void FlipTriangleOrientation(Ref<MatrixX3i> f, int f_ind);
void FixNeighborNormal(const Ref<const RowVector3i>& curr_f, Ref<MatrixX3i> f,
                       int neigh_ind);
void UnifyNeighborNormals(const Adjacency& adj, Ref<MatrixX3i> f, int f_ind,
                          vector<bool> visited);
void ReorientMeshNormals(const Ref<const MatrixX3i>& face,
                         const Ref<const MatrixX3d>& vertex,
                         Ref<MatrixX3i> face_out);
void RowwiseCross(const MatrixX3d& u, const MatrixX3d& v, MatrixX3d& uxv);

void ComputeNormals(const MatrixX3i& face, const MatrixX3d& vertex,
                    Ref<MatrixX3d> normal) {
  const Vector3i xyz_ind(0, 1, 2);
  MatrixX3d v0;
  igl::slice(vertex, face.col(0), xyz_ind, v0);

  MatrixX3d v1;
  igl::slice(vertex, face.col(1), xyz_ind, v1);

  MatrixX3d v2;
  igl::slice(vertex, face.col(2), xyz_ind, v2);

  const MatrixX3d u = v1 - v0;
  const MatrixX3d v = v2 - v0;

  Matrix<double, Dynamic, 3> n(u.rows(), 3);
  RowwiseCross(u, v, n);
}

void ReorientMeshNormals(const Ref<const MatrixX3i>& face,
                         const Ref<const MatrixX3d>& vertex,
                         Ref<MatrixX3i> face_out) {
  face_out = face;
  Adjacency adj;

  ComputeAdjacency(face, vertex.rows(), adj);
  vector<vector<int>> components;
  FindConnectedComponents(adj, components);
  for (vector<int> flist : components) {
    MatrixX3i comp_inds(flist.size(), 3);
    int i = 0;
    for (auto item : flist) {
      comp_inds.row(i) = face.row(item);
      i++;
    }

    int comp_f_ind;
    RowVector3d normal;
    Matrix<double, 1, 6> line;
    FindOneFaceNormal(vertex, comp_inds, comp_f_ind, normal, line);

    //    cout << "line" << endl << line << endl;
    //    cout << "comp_f_ind" << endl << comp_f_ind << endl;
    //    cout << "normal" << endl << normal << endl;

    if (comp_f_ind < 0) {
      continue;
    }
    assert(comp_f_ind < flist.size());

    int f_ind = flist[comp_f_ind];
    auto f = face.row(f_ind);
    RowVector3d u = vertex.row(f(1)) - vertex.row(f(0));
    RowVector3d v = vertex.row(f(2)) - vertex.row(f(0));
    RowVector3d n = u.cross(v);

    if (n.dot(normal) < 0) {
      FlipTriangleOrientation(face_out, f_ind);
    }

    vector<bool> visited(face_out.rows(), false);

    UnifyNeighborNormals(adj, face_out, f_ind, visited);
  }
}

void UnifyNeighborNormals(const Adjacency& adj, Ref<MatrixX3i> f, int f_ind,
                          vector<bool> visited) {
  // Assume f.row(f_ind) has correct orientation
  visited[f_ind] = true;
  RowVector3i curr_f = f.row(f_ind);
  for (const int& neigh_ind : adj.f_to_f[f_ind]) {
    if (visited[neigh_ind]) {
      continue;
    }
    FixNeighborNormal(curr_f, f, neigh_ind);
    UnifyNeighborNormals(adj, f, neigh_ind, visited);
  }
}

void FixNeighborNormal(const Ref<const RowVector3i>& curr_f, Ref<MatrixX3i> f,
                       int neigh_ind) {
  RowVector3i neigh_f = f.row(neigh_ind);
  //  cout << curr_f << endl;
  //  cout << neigh_ind << endl;
  //  cout << f << endl;
  int first = -1, second = -1;
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      //      cout << "comparing " << curr_f(i) << neigh_f(j) << endl;
      if (curr_f(i) == neigh_f(j)) {
        if (first < 0) {
          first = j;
        } else {
          second = j;
          if (second >= first || (first == 2 && second == 0)) {
            FlipTriangleOrientation(f, neigh_ind);
            return;
          }
          return;
        }
      }
    }
  }
}

void FlipTriangleOrientation(Ref<MatrixX3i> f, int f_ind) {
  std::swap(f(f_ind, 0), f(f_ind, 1));
}

void ComputeAdjacency(const MatrixX3i& face, size_t numv, Adjacency& adj) {
  size_t numf = face.rows();
  adj.v_to_f.resize(numv);
  adj.v_to_v.resize(numv);
  adj.f_to_f.resize(numf);

  for (size_t i = 0; i < numf; ++i) {
    const RowVector3i& f = face.row(i);
    adj.v_to_f[f(0)].insert(i);
    adj.v_to_f[f(1)].insert(i);
    adj.v_to_f[f(2)].insert(i);

    adj.v_to_v[f(0)].insert(f(1));
    adj.v_to_v[f(0)].insert(f(2));

    adj.v_to_v[f(1)].insert(f(0));
    adj.v_to_v[f(1)].insert(f(2));

    adj.v_to_v[f(2)].insert(f(0));
    adj.v_to_v[f(2)].insert(f(2));
  }

  for (size_t i = 0; i < numf; ++i) {
    const RowVector3i& f = face.row(i);
    const auto s0 = adj.v_to_f[f(0)];
    const auto s1 = adj.v_to_f[f(1)];
    const auto s2 = adj.v_to_f[f(2)];

    set<int> si01, si12, si02;
    set_intersection(s0.begin(), s0.end(), s1.begin(), s1.end(),
                     std::inserter(si01, si01.begin()));
    set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(),
                     std::inserter(si12, si12.begin()));
    set_intersection(s0.begin(), s0.end(), s2.begin(), s2.end(),
                     std::inserter(si02, si02.begin()));
    if (si01.size() > 2 || si12.size() > 2 || si02.size() > 2) {
      std::cerr << "not a manifold" << std::endl;
    }
    si01.erase(i);
    si12.erase(i);
    si02.erase(i);

    if (!si01.empty()) adj.f_to_f[i].insert(*si01.begin());
    if (!si12.empty()) adj.f_to_f[i].insert(*si12.begin());
    if (!si02.empty()) adj.f_to_f[i].insert(*si02.begin());
  }
}

void FindConnectedComponents(const Adjacency& adj,
                             vector<vector<int>>& result) {
  size_t numf = adj.f_to_f.size();

  std::queue<int> q;
  VectorXi mark(numf);
  mark.fill(-1);

  int comp_count = 0;
  for (size_t i = 0; i < numf; ++i) {
    if (mark[i] >= 0) continue;
    q.push(i);
    while (!q.empty()) {
      const int fi = q.front();
      q.pop();
      if (mark[fi] >= 0) continue;

      mark[fi] = comp_count;
      for (auto it = adj.f_to_f[fi].begin(); it != adj.f_to_f[fi].end(); it++) {
        q.push(*it);
      }

      if (result.size() <= comp_count) {
        result.push_back(vector<int>());
      }
      result[comp_count].push_back(fi);
    }
    comp_count++;
  }
}
//#include "../matlab/mexutil.h"
// using namespace mexutil;

void RowwiseCross(const MatrixX3d& u, const MatrixX3d& v, MatrixX3d& uxv) {
  uxv << (u.col(1).array() * v.col(2).array() -
          u.col(2).array() * v.col(1).array()),
      (u.col(2).array() * v.col(0).array() -
       u.col(0).array() * v.col(2).array()),
      (u.col(0).array() * v.col(1).array() -
       u.col(1).array() * v.col(0).array());
}

void FindOneFaceNormal(const MatrixX3d& vertex, const MatrixX3i& face,
                       int& face_index, RowVector3d& normal,
                       Matrix<double, 1, 6>& cline) {
  if (face.rows() < 2) {
    normal << 0, 0, 0;
    face_index = -1;
    return;
  }

  const Vector3i xyz_ind(0, 1, 2);

  MatrixX3d v0;
  igl::slice(vertex, face.col(0), xyz_ind, v0);

  MatrixX3d v1;
  igl::slice(vertex, face.col(1), xyz_ind, v1);

  MatrixX3d v2;
  igl::slice(vertex, face.col(2), xyz_ind, v2);

  //  cout << "face" << face << endl;
  //  cout << "vertex" << vertex << endl;
  //  cout << "v0" << v0 << endl;
  //  cout << "v1" << v1 << endl;
  //  cout << "v2" << v2 << endl;
  //  cout << "face.col(0)" << face.col(0) << endl;
  //  cout << "face.col(1)" << face.col(1) << endl;
  //  cout << "face.col(2)" << face.col(2) << endl;

  MatrixX3d v012(v0.rows() + v1.rows() + v2.rows(), 3);
  v012 << v0, v1, v2;

  const MatrixX3d u = v1 - v0;
  const MatrixX3d v = v2 - v0;

  Matrix<double, Dynamic, 3> n(u.rows(), 3);
  RowwiseCross(u, v, n);

  //  cout << "n" << n << endl;

  const VectorXd uv = u.cwiseProduct(v).rowwise().sum();
  const VectorXd vv = v.array().square().rowwise().sum();
  const VectorXd uu = u.array().square().rowwise().sum();

  coro<Matrix<double, 1, 6>>::pull_type line_generator([&](
      coro<Matrix<double, 1, 6>>::push_type& sink) {
    RowVector3d center = v012.colwise().mean();

    const RowVector3d unit_x(1, 0, 0);
    const RowVector3d unit_y(0, 1, 0);
    const RowVector3d unit_z(0, 0, 1);

    auto rand_offset = [](int level = 0) {
      return RowVector3d::Random(1, 3) / (15.0 - level);
    };

    Matrix<double, 1, 6> lineout;
    lineout << center, (center + unit_x);
    sink(lineout);
    lineout.rightCols(3) = center + unit_y + rand_offset();
    sink(lineout);
    lineout.rightCols(3) = center + unit_z + rand_offset();
    sink(lineout);

    RowVector3d mincoord = v012.colwise().minCoeff();
    RowVector3d maxcoord = v012.colwise().maxCoeff();
    RowVector3d range = maxcoord - mincoord;

    for (size_t i = 0; i < 7; i++) {
      lineout.leftCols(3).setRandom();
      lineout.leftCols(3) = (lineout.leftCols(3).array() / 4 + 0.5);
      lineout.leftCols(3) = lineout.leftCols(3).cwiseProduct(range) + mincoord;

      lineout.rightCols(3) = lineout.leftCols(3) + unit_x + rand_offset(i);
      sink(lineout);
      lineout.rightCols(3) = lineout.leftCols(3) + unit_y + rand_offset(i);
      sink(lineout);
      lineout.rightCols(3) = lineout.leftCols(3) + unit_z + rand_offset(i);
      sink(lineout);
    }

    //    cout << endl;

    MatrixX3d fcenters = (v0 + v1 + v2).array() / 3;

    //    cout << endl << fcenters << endl << endl;

    VectorXi rand_ind(fcenters.rows());

    //    cout << endl << rand_ind << endl << endl;

    std::iota(rand_ind.data(), rand_ind.data() + rand_ind.size(), 0);
    std::default_random_engine rg(
        (uint32_t)std::chrono::system_clock::now().time_since_epoch().count());
    std::shuffle(rand_ind.data(), rand_ind.data() + rand_ind.size(), rg);

    //    cout << endl << rand_ind << endl << endl;

    for (int i = 0; i < fcenters.rows() - 1; i += 2) {
      //      cout << rand_ind(i) << endl << endl;
      //      cout << fcenters.row(rand_ind(i)) << endl << endl;
      //      cout << fcenters.row(rand_ind(i + 1)) << endl << endl;
      lineout.leftCols(3) = fcenters.row(rand_ind(i));
      //      cout << endl << lineout << endl << endl;
      lineout.rightCols(3) = fcenters.row(rand_ind(i + 1));
      //      cout << endl << lineout << endl << endl;
      sink(lineout);
    }
  });

  for (auto line : line_generator) {
    // MATLAB:
    // pv=p1-p0; d=dot(v0-p0,n,2)./dot(pv,n,2); P=p0+pv*d; w=P-v0;
    RowVector3d pv = line.rightCols(3) - line.leftCols(3);
    VectorXd d =
        (v0.rowwise() - line.leftCols(3)).cwiseProduct(n).rowwise().sum();
    VectorXd dd = (n.array().rowwise() * pv.array()).rowwise().sum();
    d = d.cwiseQuotient(dd);
    MatrixX3d P = (d * pv).rowwise() + line.leftCols(3);
    MatrixX3d w = P - v0;

    //    cout << line << endl;

    // wv=dot(w,v,2); wu=dot(w,u,2);
    VectorXd wv = w.cwiseProduct(v).rowwise().sum();
    VectorXd wu = w.cwiseProduct(u).rowwise().sum();

    // D = uv^2-uu*vv; s=(uv*wv-vv*wu)/D; t=(uv*wu-uu*wv)/D;
    VectorXd D = uv.array().square() - uu.array() * vv.array();
    VectorXd s =
        (uv.array() * wv.array() - vv.array() * wu.array()) / D.array();
    VectorXd t =
        (uv.array() * wu.array() - uu.array() * wv.array()) / D.array();

    VectorXi inds =
        (s.array() > 0) && (t.array() > 0) && (s.array() + t.array() < 1);

    //    cout << "inds " << inds << endl;

    int num_points = inds.sum();
    if (num_points > 0 && num_points % 2 == 0) {
      int max_d = INT_MIN;
      for (int i = 0; i < d.size(); ++i) {
        if (d(i) > max_d && inds(i) == 1) {
          max_d = d(i);
          face_index = i;
        }
      }
      cline = line;
      normal = n.row(face_index);

      //      cout << normal << endl;
      //      cout << line << endl;
      //      cout << line.rightCols(3) - line.leftCols(3) << endl;
      //      cout << normal.dot(line.rightCols(3) - line.leftCols(3)) << endl;

      normal.normalize();
      if (normal.dot(line.rightCols(3) - line.leftCols(3)) < 0) {
        normal *= -1;
      }
      return;
    }
  }

  if (face.rows() < 2) {
    normal << 0, 0, 0;
    face_index = -1;
    return;
  }
}
}
}
