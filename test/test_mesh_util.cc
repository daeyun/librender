#include "gtest/gtest.h"

#include "mesh_util.h"

using namespace std;
using namespace Eigen;

TEST(ComputeAdjacency, Simple) {
  MatrixX3i face(6, 3);
  face << 0, 1, 2, 1, 2, 3, 2, 3, 4, 5, 6, 7, 6, 7, 8, 9, 10, 11;

  librender::util::Adjacency adj;
  librender::util::ComputeAdjacency(face, 12, adj);

  EXPECT_EQ(adj.v_to_f.size(), 12);
  EXPECT_EQ(adj.v_to_v.size(), 12);
  EXPECT_EQ(adj.f_to_f.size(), 6);
  EXPECT_EQ(*adj.f_to_f[0].begin(), 1);
  EXPECT_EQ(adj.f_to_f[0].size(), 1);
  EXPECT_EQ(adj.f_to_f[1].size(), 2);
}

TEST(FindConnectedComponents, Simple) {
  MatrixX3i face(6, 3);
  face << 0, 1, 2, 1, 2, 3, 2, 3, 4, 5, 6, 7, 6, 7, 8, 9, 10, 11;

  librender::util::Adjacency adj;
  librender::util::ComputeAdjacency(face, 12, adj);

  vector<vector<int>> result;
  librender::util::FindConnectedComponents(adj, result);

  EXPECT_EQ(result.size(), 3);
  EXPECT_EQ(result[0].size(), 3);
  EXPECT_EQ(result[1].size(), 2);
  EXPECT_EQ(result[2].size(), 1);
}

TEST(ReorientMeshNormals, Simple) {
  MatrixX3i face(6, 3);
  face << 0, 1, 2,

      1, 2, 3,

      2, 3, 4,

      5, 6, 7,

      6, 7, 8,

      9, 10, 11;

  MatrixX3d vertex(12, 3);
  vertex << 0, 1, 1,

      0.3, 1.3, 1,

      0.6, 0.8, 0.7,

      0.6, 0.8, 0.2,

      0.2, 1, 0,

      0.4, 0.3, 1,

      0.6, 0.4, 1,

      0.65, 0.1, 0.8,

      0.7, 0.2, 0.6,

      1.3, 1.3, 1.3,

      1.5, 1.5, 1.3,

      1.3, 1.5, 0.5;

  MatrixX3i face_out(6, 3);
  librender::util::ReorientMeshNormals(face, vertex, face_out);

  const Vector3i xyz_ind(0, 1, 2);

  MatrixX3d v0;
  igl::slice(vertex, face_out.col(0), xyz_ind, v0);

  MatrixX3d v1;
  igl::slice(vertex, face_out.col(1), xyz_ind, v1);

  MatrixX3d v2;
  igl::slice(vertex, face_out.col(2), xyz_ind, v2);

  const MatrixX3d u = v1 - v0;
  const MatrixX3d v = v2 - v0;

  Matrix<double, Dynamic, 3> n(u.rows(), 3);
  librender::util::RowwiseCross(u, v, n);
}
