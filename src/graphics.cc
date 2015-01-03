/**
 * @file graphics.cc
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright Scry is free software released under the BSD 2-Clause license.
 */
#include "graphics.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "shape.h"
#include "annotation.h"
#include "shader.h"
#include "shaders/blinn_shader.h"
#include "shader_object.h"
#include "config.h"
#include "framebuffer.h"
#include "io.h"
#include "gui.h"

namespace scry {

extern CameraParams* gui::camera_params;

/**
 * @brief
 * @param object
 * @param filename
 */
void Render(const Shape& object, const std::string& filename) {
  bool is_off_screen = true;
  if (filename.empty()) {
    is_off_screen = false;
  }

  int window_width, window_height;
  if (is_off_screen) {
    // Invisible window
    window_width = 0;
    window_height = 0;
  } else {
    window_width = config::image_width;
    window_height = config::image_height;
  }

  GLFWwindow* window =
      gui::CreateWindow(window_width, window_height, config::window_title);

  // Support experimental drivers
  glewExperimental = true;
  GLenum glew_error = glewInit();
  if (glew_error != GLEW_OK) {
    std::cerr << glewGetErrorString(glew_error) << std::endl;
    throw std::runtime_error("Failed to open GLEW.");
  }

  Framebuffer* framebuffer;
  if (is_off_screen) {
    framebuffer = new Framebuffer(config::image_width, config::image_height,
                                  config::num_msaa_samples);
    framebuffer->Bind();
    // Required for the new framebuffer object.
    glViewport(0, 0, config::image_width, config::image_height);
    glEnable(GL_MULTISAMPLE);
  }

  glm::vec4 bg = config::background;
  glClearColor(bg.r, bg.g, bg.b, bg.a);

  // Enable depth test.
  glEnable(GL_DEPTH_TEST);

  // Accept fragment if it closer to the camera than the former one.
  glDepthFunc(GL_LESS);

  CameraParams cam_params;
  cam_params.target = config::target;
  cam_params.up_ang = config::up_ang;
  cam_params.el = config::el;
  cam_params.az = config::az;
  cam_params.r = config::r;

  gui::camera_params = &cam_params;

  ShaderViewParams view_params;

  ShaderProperties shader_properties;
  InitializeShader(shader_properties);

  GLuint shader_id = shader::Shader(kBlinnShader);

  ShaderObject drawable_object(&object, shader_id, NULL);

  float min_z = arma::min(object.v.row(2));
  Annotation annotation(min_z);

  do {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ComputeMatrices(cam_params, view_params);

    UpdateShaderView(view_params, shader_properties);

    annotation.draw(view_params.projection_mat, view_params.view_mat,
                    view_params.model_mat);
    drawable_object.draw(shader_properties);

    glfwSwapBuffers(window);
    glfwWaitEvents();
  } while (!is_off_screen && !glfwWindowShouldClose(window));

  if (is_off_screen) {
    // Read pixel values from the framebuffer.
    size_t buffer_size = framebuffer->Size();
    uint8_t* pixels = (uint8_t*)malloc(buffer_size);
    framebuffer->ReadPixels(pixels);

    io::SaveAsPNG(filename, pixels, config::image_width, config::image_height);
    free(pixels);
    framebuffer->Unbind();

    delete framebuffer;
  }

  glDeleteProgram(shader_id);
  glfwTerminate();
}

/**
 * @brief
 * @param[in] view_params
 * @param[out] shader_properties
 */
void UpdateShaderView(const ShaderViewParams& view_params,
                      ShaderProperties& shader_properties) {
  shader_properties.mv = view_params.view_mat * view_params.model_mat;
  shader_properties.mvp = view_params.projection_mat * shader_properties.mv;
  shader_properties.eye_direction = view_params.eye_direction;
  shader_properties.normal_mat =
      glm::transpose(glm::inverse(shader_properties.mv));
}

/**
 * @brief
 * @param shader_properties
 */
void InitializeShader(ShaderProperties& shader_properties) {
  std::vector<LightProperties> lights;
  LightProperties light;
  light.light_position = config::light_position;
  light.ambient = config::ambient;
  light.constant_attenuation = config::constant_attenuation;
  light.linear_attenuation = config::linear_attenuation;
  light.quadratic_attenuation = config::quadratic_attenuation;
  lights.push_back(light);

  shader_properties.lights = lights;
  shader_properties.shininess = config::shininess;
  shader_properties.strength = config::strength;
}

/**
 * @brief Rotate a vector around an axis.
 * @param[in] axis Axis vector to rotate around.
 * @param[in] angle Angle in radians.
 * @param[out] vector Vector to rotate.
 */
void RotateVector(const glm::vec3& axis, const float angle, glm::vec3& vector) {
  vector = glm::vec3(glm::rotate(angle, axis) * glm::vec4(vector, 1));
}

/**
 * @brief
 * @param params
 * @param output
 */
void ComputeMatrices(const CameraParams& params, ShaderViewParams& output) {
  float fov = glm::radians(config::fov);

  glm::vec3 position = glm::vec3(params.r * sin(params.el) * cos(params.az),
                                 params.r * sin(params.el) * sin(params.az),
                                 params.r * cos(params.el));

  glm::vec3 up = glm::vec3(sin(params.el - kPi / 2) * cos(params.az),
                           sin(params.el - kPi / 2) * sin(params.az),
                           cos(params.el - kPi / 2));

  glm::vec3 lookat = glm::normalize(params.target - position);
  RotateVector(lookat, params.up_ang, up);

  output.projection_mat =
      glm::perspective(fov, ((float)config::image_width) / config::image_height,
                       config::near, config::far);
  output.view_mat = glm::lookAt(position, position + lookat, up);
  output.model_mat = glm::mat4(1.0);
  output.eye_direction = -lookat;
}
}
