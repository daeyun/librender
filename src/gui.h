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

namespace scry {
namespace gui {

enum Key { SHIFT, CONTROL, ALT, UP, DOWN, LEFT, RIGHT };

void Initialize();
GLFWwindow* CreateWindow(int width, int height, const std::string& title);
void UpdateWindowThread();
void InitWindowControls(GLFWwindow* window);
void KeyEventHandler(GLFWwindow* window, int key, int scancode, int action,
                     int mods);

static std::unordered_map<GLFWwindow*, std::set<Key>> keys_pressed;
static std::mutex keys_pressed_mutex;

static int fps = 30;
static bool is_initialized = false;
}
}
