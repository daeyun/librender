/**
 * @file framebuffer.h
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright Scry is free software released under the BSD 2-Clause license.
 */
#pragma once

#include <stdexcept>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <GL/glew.h>

namespace scry {

class Framebuffer {
 public:
  Framebuffer(int width, int height, int num_msaa_samples = 4);
  ~Framebuffer();
  void Bind();
  void Unbind();
  size_t Size();
  void ReadPixels(uint8_t* pixels);

 private:
  struct FBO {
    GLuint id = 0, color_buf = 0, depth_buf = 0;
  };

  void InitializeFBO(FBO& fbo, int num_msaa_samples);
  void InitializeRBO(GLenum target, GLenum format, int num_msaa_samples,
                     GLuint& buf);
  std::string GetStatusErrorString(GLenum status);
  void CheckStatus();

  int w_, h_;
  GLint prev_buffer_id_;
  FBO draw_fbo_, read_fbo_;

  GLenum color_format_ = GL_RGBA;
  size_t pixel_size_ = 4;  // 3 for GL_RGB
};
}
