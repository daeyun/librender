#ifndef MESH_H_
#define MESH_H_

#include <armadillo>

class Mesh {
   public:
    Mesh();

    arma::fmat v;
    arma::umat f;
    arma::fmat vn;

   private:
};

#endif  // MESH_H_
