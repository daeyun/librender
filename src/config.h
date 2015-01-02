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

enum Axis { X, Y, Z };

const std::string kAppName = "Scry";
const std::string kAppDesc = "Scry is a tool for previewing mesh files.";
const std::string kConfigFileName = ".scryrc";

namespace config {

extern bool will_normalize;
extern glm::vec3 camera;
extern glm::vec3 target;
extern float fov;
extern Axis up_axis;
extern bool are_axes_visible;
extern glm::vec4 background;
extern glm::vec3 light_position;
extern glm::vec3 ambient;
extern float constant_attenuation;
extern float linear_attenuation;
extern float quadratic_attenuation;
extern float shininess;
extern float strength;
extern float window_control_speed;
extern float up_ang;
extern float el;
extern float az;
extern float r;
extern float near;
extern float far;
extern int image_width;
extern int image_height;
extern std::string window_title;
extern arma::fvec color;
extern int num_msaa_samples;
extern std::vector<std::string> mesh_files;
extern std::string outfile;
}
}
