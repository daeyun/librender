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

glm::vec3 camera = glm::vec3(1, 0, 0);
glm::vec3 target = glm::vec3(0, 0, 0);
float fov = 60;

Axis up_axis = Y;
bool will_normalize = true;
bool are_axes_visible = false;
glm::vec4 background = glm::vec4(1, 1, 1, 1);

glm::vec3 light_position = glm::vec3(2, 0, 1);
glm::vec3 ambient = glm::vec3(0.3, 0.3, 0.3);
float constant_attenuation = 1;
float linear_attenuation = 0.25;
float quadratic_attenuation = 0.15;
float shininess = 20;
float strength = 1;

float window_control_speed = 1;
float up_ang = 0;
float el = 0;
float az = 0;
float r = 1.5;

float near = 0.1;
float far = 100;

int image_width = 800;
int image_height = 600;
std::string window_title = "Scry";

int num_msaa_samples = 4;

arma::fvec color = arma::fvec({1, 0, 0, 1});

std::vector<std::string> mesh_files;

std::string outfile;
}
}
