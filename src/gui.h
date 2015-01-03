/**
 * @file gui.h
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright Scry is free software released under the BSD 2-Clause license.
 */
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <set>
#include <mutex>
#include "graphics.h"

namespace scry {
namespace gui {

void Initialize();
GLFWwindow* CreateWindow(int width, int height, const std::string& title);
void UpdateWindowThread();
void InitWindowControls(GLFWwindow* window);
void KeyEventHandler(GLFWwindow* window, int key, int scancode, int action,
                     int mods);

extern CameraParams* camera_params;
}
}