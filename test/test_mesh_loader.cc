#include "mesh_loader.h"
#include "shape.h"
#include "gtest/gtest.h"

const double kMatEqTol = 1e-4;

using namespace librender;

bool is_close(arma::fmat& X, arma::fmat& Y) {
  if (arma::max(arma::vectorise(arma::abs(X - Y))) < kMatEqTol) return true;
  return false;
}

TEST(SanityCheck, AlwaysTrue) { EXPECT_TRUE(42); }