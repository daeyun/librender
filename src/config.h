/**
 * @file config.h
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright Scry is free software released under the BSD 2-Clause license.
 */
#pragma once

#include <string>
#include <vector>
#include <armadillo>
#include <glm/glm.hpp>

namespace scry {

const std::string kAppName = "Scry";
const std::string kAppDesc = "Scry is a tool for previewing mesh files.";
const std::string kConfigFileName = ".scryrc";

namespace config {

extern float window_control_speed;
extern std::string window_title;
extern bool is_verbose;

}
}
