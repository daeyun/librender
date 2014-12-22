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
