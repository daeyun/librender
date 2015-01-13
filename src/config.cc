/**
 * @file config.cc
 * @brief
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright librender is free software released under the BSD 2-Clause
 * license.
 */
#include "config.h"

#include <glm/glm.hpp>
#include "yaml-cpp/yaml.h"

namespace librender {
namespace config {

float window_control_speed = 1;
std::string window_title = "librender";
bool is_verbose = true;
}
}
