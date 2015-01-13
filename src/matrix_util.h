#include <armadillo>
#include <glm/glm.hpp>
#include "debug.h"

namespace librender {
namespace util {

/**
 * @brief Add a row vector of ones to the matrix
 * @param a
 * @return
 */
arma::fmat ToHom(const arma::fmat& a) {
  return arma::join_vert(a, arma::ones<arma::fmat>(1, a.n_cols));
}

arma::fmat FromHom(arma::fmat a) {
  a.rows(0, 2).each_row() /= a.row(3);
  return a.rows(0, 2);
}

arma::fmat MatMult(const glm::mat4x4& a, const arma::fmat& b) {
  return FromHom(arma::fmat(&a[0][0], 4, 4) * ToHom(b));
}

arma::fmat MatMult(const glm::mat3x3& a, const arma::fmat& b) {
  return arma::fmat(&a[0][0], 3, 3) * b;
}

glm::vec3 MatMult(const glm::mat4x4& a, const glm::vec3 b) {
  auto c = a * glm::vec4(b, 1);
  return glm::vec3(c) / c.w;
}

void CopyMatInPlace(const arma::fmat& from, arma::fmat& to) {
  size_t size = to.n_elem * sizeof(to[0]);
  memcpy(to.memptr(), from.memptr(), size);
}
}
}
