#pragma once

#include <armadillo>

enum ScryObjectType {kTriangleMesh, kLines};

struct ScryObject {
    ScryObjectType type;

    arma::fmat v;
    arma::umat f;
    arma::fmat vn;
    arma::fmat uv;
    arma::fmat cdata;
};
