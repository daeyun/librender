#include "mesh_loader.h"
#include "gtest/gtest.h"
#include <armadillo>

const double kMatEqTol = 1e-4;

bool is_close(arma::fmat& X, arma::fmat& Y) {
    if (arma::max(arma::vectorise(arma::abs(X-Y))) < kMatEqTol)
        return true;
    return false;
}

TEST(SanityCheck, AlwaysTrue) {
    EXPECT_TRUE(42);
}

TEST(TestNormalizeCoords, Cube) {
    arma::fmat v = {-1, -1, -1, -1, -1, -1, 1, 1, 1, 0, 0, 0};
    v.reshape(3, 2);

    normalize_coords(v);

    arma::fmat u = {-0.5, -0.5, -0.5,-0.5, -0.5, -0.5, 0.5, 0.5, 0.5, 0, 0, 0};
    u.reshape(3, 2);

    EXPECT_TRUE(is_close(v, u));
}

TEST(TestNormalizeCoords, CheckScalePreserved) {
    arma::fmat v = {0, 0, 0, 2, 1, 1};
    v.reshape(3, 2);

    normalize_coords(v);

    arma::fmat u = {-0.5, -0.25, -0.25, 0.5, 0.25, 0.25};
    u.reshape(3, 2);

    EXPECT_TRUE(is_close(v, u));
}

TEST(TestCrossCol, Simple) {
    arma::fmat a = {0, 0, 0,  1, 1, 1,  2, 2, 2,  0, 1, 2};
    arma::fmat b = {1, 0, 0,  0, 0, 0,  4, 3, 0,  2, 1, 1};
    a.reshape(3, 4);
    b.reshape(3, 4);

    arma::fmat c;
    cross_col(a, b, c);

    arma::fmat expected = {0, 0, 0, 0, 0, 0, -6, 8, -2, -1, 4, -2};
    expected.reshape(3, 4);

    EXPECT_TRUE(is_close(c, expected));
}

TEST(TestLoadObj, Simple) {
    Mesh mesh;
    load_obj("../test/resources/teapot.obj", mesh);

    EXPECT_EQ(mesh.vn.n_cols, mesh.v.n_cols);
    EXPECT_EQ(mesh.vn.n_rows, mesh.v.n_rows);
    EXPECT_EQ(mesh.f.n_rows, mesh.v.n_rows);
}
