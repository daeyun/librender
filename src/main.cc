/**
 * @file main.cc
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright Scry is free software released under the BSD 2-Clause license.
 */
#include "main.h"

#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include "tiny_obj_loader.h"
#include "config.h"
#include "mesh_loader.h"
#include "shape.h"
#include "graphics.h"
#include "scry.h"

int main(int argc, char* argv[]) {
  scry::config::InitFromMainArgs(argc, argv);

  for (auto mesh_file : scry::config::mesh_files) {
    scry::Shape mesh;
    scry::LoadObj(mesh_file, mesh);
    scry::Render(mesh, scry::config::outfile);
  }
}
