#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>
#include <vector>
#include <armadillo>

enum Axis { X, Y, Z };

namespace config {
    extern bool will_normalize;
    extern arma::vec camera;
    extern arma::vec target;
    extern Axis up_axis;

    void load_config_file(std::string config_file);
}

#endif  // CONFIG_H_
