/**
 * @file framebuffer.cc
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright Scry is free software released under the BSD 2-Clause license.
 */
#include "framebuffer.h"

namespace scry {

/**
 * @brief This class is a wrapper for OpenGL framebuffer objects used for
 *        off-screen rendering.
 * @param width,height Size of the image in pixels.
 * @param num_msaa_samples Multisample anti-aliasing (MSAA) level. Typically
 *        2, 4, or 8.
 */
Framebuffer::Framebuffer(int width, int height, int num_msaa_samples) {
  w_ = width;
  h_ = height;

  // Backup previous buffer IDs.
  GLint prevFBO, prevRBO;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFBO);
  glGetIntegerv(GL_RENDERBUFFER_BINDING, &prevRBO);

  // Initialize both multisampled and normal buffers for anti-aliasing support.
  InitializeFBO(draw_fbo_, num_msaa_samples);
  InitializeFBO(read_fbo_, 0);

  // Restore previous buffer IDs.
  glBindFramebuffer(GL_FRAMEBUFFER, prevFBO);
  glBindRenderbuffer(GL_RENDERBUFFER, prevRBO);
}

/**
 * @brief Delete the framebuffer and renderbuffer objects.
 */
Framebuffer::~Framebuffer() {
  glDeleteFramebuffers(1, &draw_fbo_.id);
  glDeleteRenderbuffers(1, &draw_fbo_.color_buf);
  glDeleteRenderbuffers(1, &draw_fbo_.depth_buf);
  glDeleteFramebuffers(1, &read_fbo_.id);
  glDeleteRenderbuffers(1, &read_fbo_.color_buf);
  glDeleteRenderbuffers(1, &read_fbo_.depth_buf);
}

/**
 * @brief Throw a runtime exception if there is an incomplete buffer.
 */
void Framebuffer::CheckStatus() {
  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) {
    throw std::runtime_error(std::string("Framebuffer error: ") +
                             GetStatusErrorString(status));
  }
}

/**
 * @brief Initialize a framebuffer. To support anti-aliasing, one would need
 *        draw the object to multisampled buffers and then copy the pixels to
 *        a normal buffer because \c glReadPixels cannot read directly from a
 *        multisampled buffer.
 * @param fbo
 * @param num_msaa_samples
 */
void Framebuffer::InitializeFBO(FBO& fbo, int num_msaa_samples) {
  glGenFramebuffers(1, &fbo.id);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo.id);
  InitializeRBO(GL_COLOR_ATTACHMENT0, color_format_, num_msaa_samples,
                fbo.color_buf);
  InitializeRBO(GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT24, num_msaa_samples,
                fbo.depth_buf);

  CheckStatus();
}

/**
 * @brief Initialize a renderbuffer. If \a num_msaa_samples is greater than 0,
 *        it will be a multisampled buffer.
 * @param target GL_COLOR_ATTACHMENTi for color buffer and GL_DEPTH_ATTACHMENT
 *               for depth buffer.
 * @param format GL_RGBA. GL_BGRA gives an GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT
 *               error for some reason. GL_DEPTH_COMPONENT24 for depth buffer.
 * @param num_msaa_samples
 * @param buf
 */
void Framebuffer::InitializeRBO(GLenum target, GLenum format,
                                int num_msaa_samples, GLuint& buf) {
  glGenRenderbuffers(1, &buf);
  glBindRenderbuffer(GL_RENDERBUFFER, buf);
  if (num_msaa_samples > 0) {
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, num_msaa_samples, format,
                                     w_, h_);
  } else {
    glRenderbufferStorage(GL_RENDERBUFFER, format, w_, h_);
  }
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, target, GL_RENDERBUFFER, buf);
}

/**
 * @brief Backup the previous buffer and bind the draw buffer for writing.
 */
void Framebuffer::Bind() {
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prev_buffer_id_);
  glBindFramebuffer(GL_FRAMEBUFFER, draw_fbo_.id);
  CheckStatus();
}

/**
 * @brief Restore the previous buffer. There is no need to call this function
 *        if the previous buffer is not needed.
 */
void Framebuffer::Unbind() {
  GLint current_buffer_id;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &current_buffer_id);
  if ((GLuint)current_buffer_id == draw_fbo_.id)
    glBindFramebuffer(GL_FRAMEBUFFER, prev_buffer_id_);
}

/**
 * @brief Return the size of the buffer in bytes. Each R, G, B, A channel takes
 *        up one byte. ReadPixels() needs this amount of memory allocated.
 * @return Size
 * @see ReadPixels
 */
size_t Framebuffer::Size() { return sizeof(uint8_t) * w_ * h_ * pixel_size_; }

/**
 * @brief Read color pixels from the framebuffer object.
 *
 * @param[out] pixels A pointer to a preallocated block of memory. Output will
 *             be in R, G, B, A, R, G, B, ... order from the <b>bottom left</a>
 *             corner.
 * @see Size
 */
void Framebuffer::ReadPixels(uint8_t* pixels) {
  // Block until all GL execution is complete.
  glFinish();

  // Read from the multisampled draw buffer and write the antialiased output to
  // the read buffer.
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, read_fbo_.id);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, draw_fbo_.id);

  glBlitFramebuffer(0, 0, w_, h_, 0, 0, w_, h_, GL_COLOR_BUFFER_BIT, GL_LINEAR);

  // Read the color pixels from the read buffer.
  glBindFramebuffer(GL_FRAMEBUFFER, read_fbo_.id);
  CheckStatus();

  glReadBuffer(GL_COLOR_ATTACHMENT0);
  glReadPixels(0, 0, w_, h_, color_format_, GL_UNSIGNED_BYTE, pixels);
}

/**
 * @brief Translate the OpenGL framebuffer error codes to English.
 * @param status
 * @return
 */
std::string Framebuffer::GetStatusErrorString(GLenum status) {
  switch (status) {
    case GL_FRAMEBUFFER_COMPLETE:
      throw std::runtime_error("GL_FRAMEBUFFER_COMPLETE is not an error.");
    case GL_FRAMEBUFFER_UNDEFINED:
      return "Default framebuffer does not exist.";
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
      return "Incomplete framebuffer attachment point.";
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
      return "Framebuffer needs at least one image attached to it.";
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
      return "Incomplete draw buffer.";
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
      return "Incomplete read buffer.";
    case GL_FRAMEBUFFER_UNSUPPORTED:
      return "Unsupported format.";
    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
      return "Incomplete multisample buffers.";
    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
      return "Incomplete layer targets.";
    default:
      return std::string("Framebuffer status: ") + std::to_string(status);
  }
}
}
