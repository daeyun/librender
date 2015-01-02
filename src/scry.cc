/**
 * @file scry.cc
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright Scry is free software released under the BSD 2-Clause license.
 */
#include "scry.h"

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <pwd.h>
#include <yaml-cpp/yaml.h>
#include "io.h"
#include "config.h"

namespace scry {
namespace config {
namespace po = boost::program_options;
namespace fs = boost::filesystem;

int InitFromMainArgs(int argc, char** argv) {
  std::string homedir = getenv("HOME");
  if (homedir.empty()) {
    homedir = getpwuid(getuid())->pw_dir;
  }

  fs::path default_rc(homedir);
  default_rc /= kConfigFileName;
  // Declare the supported options.
  po::options_description desc("Options");
  auto config_opt =
      po::value<std::string>()->default_value(default_rc.string());
  auto resolution_opt = po::value<int>()->default_value(640);
  auto mesh_files_opt = po::value<std::vector<std::string>>()->required();
  auto outfile_opt = po::value<std::string>()->default_value("");
  desc.add_options()

      ("version,v", "print version string")

      ("help", "produce help message")

      ("config-file,c", config_opt, "path to configuration file")

      ("resolution,r", resolution_opt, "resolution of output images")

      ("outfile,o", outfile_opt, "path to output image. .png")

      ("mesh-files", mesh_files_opt, "supported file types: .obj");

  po::positional_options_description positional_opts;
  positional_opts.add("mesh-files", -1);
  po::variables_map vm;

  try {
    po::store(po::command_line_parser(argc, argv)
                  .options(desc)
                  .positional(positional_opts)
                  .run(),
              vm);

    if (vm.count("version")) {
      std::cout << kAppName << " " << VERSION << std::endl;
      return 1;
    }

    if (vm.count("help") || !vm.count("mesh-files")) {
      std::cout << kAppDesc << std::endl << std::endl << desc;
      return 1;
    }

    po::notify(vm);

    std::string config_file = vm["config-file"].as<std::string>();

    config::mesh_files = vm["mesh-files"].as<std::vector<std::string>>();
    config::image_height = config::image_width = vm["resolution"].as<int>();
    config::outfile = vm["outfile"].as<std::string>();

  } catch (std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl << std::endl << desc;
    return -1;
  }

  return 0;
}

void InitFromFile(const std::string& filename) {
  // TODO
  std::string config_file = scry::io::UpSearch(fs::path(filename).parent_path(),
                                               scry::kConfigFileName);
  YAML::Node config = YAML::LoadFile(config_file);
  config["will_normalize"].as<bool>();
}
}
}
