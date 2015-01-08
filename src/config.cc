/**
 * @file config.cc
 * @brief
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright Scry is free software released under the BSD 2-Clause license.
 */
#include "config.h"

#include <glm/glm.hpp>
#include "yaml-cpp/yaml.h"

// Default values

namespace scry {
namespace config {

float window_control_speed = 1;
std::string window_title = "Scry";
bool is_verbose = true;

}
}
