#include "controls.h"

#define GLM_FORCE_RADIANS

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/constants.hpp>
#include "config.h"

using namespace glm;

extern GLFWwindow* window;

const float kPi = glm::pi<float>();

mat4 ViewMatrix;
mat4 ProjectionMatrix;
mat4 NormalMatrix;
vec3 EyeDirection;

mat4 getViewMatrix() { return ViewMatrix; }
mat4 getProjectionMatrix() { return ProjectionMatrix; }
vec3 getEyeDirection() { return EyeDirection; }

float fov = glm::radians(config::fov);
float speed = config::window_control_speed;

vec3 target = config::target;
float up_ang = config::up_ang;
float el = config::el;
float az = config::az;
float r = config::r;

vec3 position = vec3(r*sin(el)*cos(az), r*sin(el)*sin(az), r*cos(el));
vec3 up = vec3(sin(el-kPi/2)*cos(az), sin(el-kPi/2)*sin(az), cos(el-kPi/2));
vec3 lookat = glm::normalize(target-position);


bool did_change = true;

/**
 * Rotate a vector around an axis using GLM functions.
 */
void rotate_vec3(const glm::vec3 &axis, const float angle, glm::vec3 &v) {
    v = glm::vec3(glm::rotate(angle, axis)*glm::vec4(v, 1));
}

bool computeMatricesFromInputs() {
    static double lastTime = glfwGetTime();

    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    float delta = deltaTime * speed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            r += delta;
            did_change = true;
        } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            r -= delta;
            did_change = true;
        }

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            up_ang += delta;
            did_change = true;
        } else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            up_ang -= delta;
            did_change = true;
        }
    } else {
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            el += delta;
            did_change = true;
        } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            el -= delta;
            did_change = true;
        }

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            az += delta;
            did_change = true;
        } else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            az -= delta;
            did_change = true;
        }
    }

    position = vec3(r*sin(el)*cos(az), r*sin(el)*sin(az), r*cos(el));
    up = vec3(sin(el-kPi/2)*cos(az), sin(el-kPi/2)*sin(az), cos(el-kPi/2));
    lookat = glm::normalize(target-position);
    rotate_vec3(lookat, up_ang, up);

    ProjectionMatrix = glm::perspective(fov, ((float)config::window_width)/config::window_height,
                                        config::near, config::far);
    ViewMatrix = glm::lookAt(position, position + lookat, up);
    EyeDirection = glm::normalize(position - target);

    lastTime = currentTime;
    if (!did_change)
        return false;

    did_change = false;
    return true;
}
