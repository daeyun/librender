/**
 * @file gui.cc
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright librender is free software released under the BSD 2-Clause
 * license.
 */
#include "gui.h"

#define GLM_FORCE_RADIANS

#include <thread>
#include <condition_variable>
#include <glm/gtx/string_cast.hpp>
#include "config.h"

namespace librender {
namespace gui {

RenderParams* render_params;

/**
 * @brief Create a GLFW window. Invisible if width or height is 0.
 * @param width Window width in pixels.
 * @param height Window height in pixels.
 * @param title Title of the window.
 * @return Pointer to the created GLFW window.
 */
GLFWwindow* CreateWindow(int width, int height, const std::string& title,
                         const RenderParams& render_params) {
  if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW");

  bool is_visible = true;
  if (width == 0 || height == 0) {
    is_visible = false;
    height = width = 1;
  }

  // Antialiasing
  glfwWindowHint(GLFW_SAMPLES, render_params.num_msaa_samples);

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

void InitWindowControls(GLFWwindow* window) {
  glfwSetKeyCallback(window, KeyEventHandler);
}

void KeyEventHandler(GLFWwindow* window, int key, int scancode, int action,
                     int mods) {
  float delta = librender::config::window_control_speed *
                std::sqrt(render_params->r) * 0.07;
  if (action & (GLFW_PRESS | GLFW_REPEAT)) switch (key) {
      case GLFW_KEY_UP:
        if (mods & GLFW_MOD_SHIFT)
          render_params->r += delta;
        else
          render_params->el += delta;
        break;
      case GLFW_KEY_DOWN:
        if (mods & GLFW_MOD_SHIFT)
          render_params->r -= delta;
        else
          render_params->el -= delta;
        break;
      case GLFW_KEY_LEFT:
        if (mods & GLFW_MOD_SHIFT)
          render_params->up_angle -= delta * 20;
        else
          render_params->az -= delta;
        break;
      case GLFW_KEY_RIGHT:
        if (mods & GLFW_MOD_SHIFT)
          render_params->up_angle += delta * 20;
        else
          render_params->az += delta;
        break;
      case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, GL_TRUE);
      default:
        return;
    }
}
}
}
