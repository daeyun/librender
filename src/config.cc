#include "config.h"

#include "yaml-cpp/yaml.h"

namespace config {

bool will_normalize = true;
arma::vec camera = {1, 0, 0};
arma::vec target = {0, 0, 0};
Axis up_axis = X;
}

void config::load_config_file(std::string config_file) {
    // TODO
    // YAML::Node config = YAML::LoadFile("sample.yaml");
    // config["normalize"].as<bool>();
}
