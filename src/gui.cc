/**
 * @file gui.cc
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright Scry is free software released under the BSD 2-Clause license.
 */
#include "gui.h"

#define GLM_FORCE_RADIANS

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/constants.hpp>
#include "config.h"

namespace scry {
namespace gui {

void Initialize() {
  if (!is_initialized) {
    is_initialized = true;
    std::thread update_thread(UpdateWindowThread);
  }
}

/**
 * @brief Create a GLFW window. Invisible if width or height is 0.
 *
 * @param width Window width in pixels.
 * @param height Window height in pixels.
 * @param title Title of the window.
 *
 * @return Pointer to the created GLFW window.
 */
GLFWwindow* CreateWindow(int width, int height, const std::string& title) {
  if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW");

  bool is_visible = true;
  if (width == 0 || height == 0) {
    is_visible = false;
    height = width = 1;
  }

  // Antialiasing
  glfwWindowHint(GLFW_SAMPLES, config::num_msaa_samples);

  // OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  if (!is_visible) glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

  // For OS X
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // Open a window and create its OpenGL context.
  GLFWwindow* window =
      glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

  if (window == NULL) {
    glfwTerminate();
    throw std::runtime_error("Failed to open GLFW window.");
  }

  if (is_visible) InitWindowControls(window);

  glfwMakeContextCurrent(window);

  return window;
}

void UpdateWindowThread() {
  // draw

  std::this_thread::sleep_for(std::chrono::milliseconds(1000 / fps));
}

void InitWindowControls(GLFWwindow* window) {
  // keys_pressed.insert(window, std::set<Key>());
  // glfwSetKeyCallback(window, KeyEventHandler);
}

void KeyEventHandler(GLFWwindow* window, int key, int scancode, int action,
                     int mods) {
  Key scry_key;

  switch (key) {
    case GLFW_KEY_UP:
      scry_key = UP;
      break;
    case GLFW_KEY_DOWN:
      scry_key = DOWN;
      break;
    case GLFW_KEY_LEFT:
      scry_key = LEFT;
      break;
    case GLFW_KEY_RIGHT:
      scry_key = RIGHT;
      break;
    case GLFW_KEY_LEFT_SHIFT:
      scry_key = SHIFT;
      break;
    case GLFW_KEY_LEFT_CONTROL:
      scry_key = CONTROL;
      break;
    case GLFW_KEY_LEFT_ALT:
      scry_key = ALT;
      break;
    default:
      return;
  }

  // keys_pressed_mutex.lock();
  // try {
  //  switch (action) {
  //    case GLFW_PRESS:
  //      keys_pressed[window].insert(scry_key);
  //      break;
  //    case GLFW_RELEASE:
  //      keys_pressed[window].erase(scry_key);
  //      break;
  //    default:
  //      break;
  //  }
  //} catch (...) {
  //}
  // keys_pressed_mutex.unlock();
}

// float control_speed = config::window_control_speed;
//
// bool UpdateCameraParamsFromInput(GLFWwindow* window, CameraParams params) {
//  static double lastTime = glfwGetTime();
//
//  double currentTime = glfwGetTime();
//  float deltaTime = float(currentTime - lastTime);
//
//  float delta = deltaTime * control_speed;
//  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
//    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
//      params.r += delta;
//      did_change = true;
//    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
//      params.r -= delta;
//      did_change = true;
//    }
//
//    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
//      params.up_ang += delta;
//      did_change = true;
//    } else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
//      params.up_ang -= delta;
//      did_change = true;
//    }
//  } else {
//    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
//      params.el += delta;
//      did_change = true;
//    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
//      params.el -= delta;
//      did_change = true;
//    }
//
//    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
//      params.az += delta;
//      did_change = true;
//    } else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
//      params.az -= delta;
//      did_change = true;
//    }
//  }
//
//  lastTime = currentTime;
//  if (!did_change) return false;
//
//  did_change = false;
//  return true;
//}
}
}
