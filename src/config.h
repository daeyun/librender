#pragma once

#include <string>
#include <vector>
#include <armadillo>
#include <glm/glm.hpp>

enum Axis { X, Y, Z };

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
    extern int window_width;
    extern int window_height;
    extern std::string window_title;
    extern arma::fvec color;

    void load_config_file(std::string config_file);
}
