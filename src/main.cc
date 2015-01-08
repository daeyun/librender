/**
 * @file main.cc
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright Scry is free software released under the BSD 2-Clause license.
 */
#include "main.h"

#include <iostream>
#include <vector>
#include "config.h"
#include "mesh_loader.h"
#include "graphics.h"
#include "scry.h"

int main(int argc, char* argv[]) {
  std::vector<scry::RenderParams> all_params;
  scry::config::InitFromMainArgs(argc, argv, all_params);

  for (scry::RenderParams& params: all_params) {
    if (scry::config::is_verbose) {
      std::cout << params.in_filename << std::endl;
    }
    scry::Shape mesh;
    scry::LoadObj(params, mesh);
    scry::Render(mesh, params);
  }
}
