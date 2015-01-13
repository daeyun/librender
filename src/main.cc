/**
 * @file main.cc
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright librender is free software released under the BSD 2-Clause
 * license.
 */
#include "main.h"
#include <iostream>
#include <vector>
#include "config.h"
#include "mesh_loader.h"
#include "graphics.h"
#include "librender.h"
#include "debug.h"

int main(int argc, char* argv[]) {
  std::vector<librender::RenderParams> all_params;
  librender::config::InitFromMainArgs(argc, argv, all_params);

  for (librender::RenderParams& params : all_params) {
    if (librender::config::is_verbose) {
      std::cout << params.in_filename << std::endl;
    }
    librender::Shape mesh;
    librender::LoadObj(params, mesh);
    librender::Render(mesh, params);
  }
}
