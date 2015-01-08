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

namespace po = boost::program_options;
namespace fs = boost::filesystem;
namespace scry {
namespace config {

int InitFromMainArgs(int argc, char* argv[],
                     std::vector<RenderParams>& all_params) {
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
  auto resolution_opt = po::value<float>();
  auto mesh_files_opt = po::value<std::vector<std::string>>()->required();
  auto out_opt = po::value<std::string>();
  desc.add_options()

      ("version,v", "print version string")

      ("help", "produce help message")

      ("config-file,c", config_opt, "path to configuration file")

      ("resolution,r", resolution_opt, "resolution of output images")

      ("out,o", out_opt, "path to output directory or filename. .png")

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

    // $HOME/.scryrc by default
    std::string config_file = vm["config-file"].as<std::string>();

    auto in_files = vm["mesh-files"].as<std::vector<std::string>>();

    for (const std::string& filename : in_files) {
      RenderParams params;
      params.in_filename = filename;
      InitFromFile(config_file, params);
      all_params.push_back(params);
    }

    if (vm.count("resolution")) {
      float resolution = vm["resolution"].as<float>();
      float scale = 1;
      for (RenderParams& params : all_params) {
        // if resolution is an integer and greater than 16, use it as a raw
        // pixel value. otherwise use it as a scalar value.
        if (std::floorf(resolution) == resolution && resolution > 16) {
          scale =
              resolution / std::max(params.image_width, params.image_height);
        } else {
          scale = resolution;
        }
        params.image_width *= scale;
        params.image_height *= scale;
      }
    }

    if (vm.count("out")) {
      fs::path out_path = vm["out"].as<std::string>();
      for (RenderParams& params : all_params) {
        std::string out_dir;
        if (out_path.has_filename() &&
            out_path.filename().string().length() > 1) {
          if (all_params.size() == 1) {
            params.out_filename = out_path.string();
            break;
          }

          fs::path p;
          p = out_path;
          p.remove_filename();
          out_dir = p.string();
        } else {
          out_dir = out_path.string();
        }

        // change the output directory
        if (params.out_filename.empty()) {
          params.out_filename =
              (out_dir / fs::path(params.in_filename).stem()).string() + ".png";
        } else {
          params.out_filename =
              (out_dir / fs::path(params.out_filename).filename()).string();
        }
      }
    }

  } catch (std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl << std::endl << desc;
    return -1;
  }

  return 0;
}

void InitFromFile(const std::string& filename, RenderParams& params) {
  std::string config_file = scry::io::UpSearch(fs::path(filename).parent_path(),
                                               scry::kConfigFileName);
  if (config_file.empty()) return;

  YAML::Node config = YAML::LoadFile(config_file);

  // Camera location
  if (config["camera-position"].IsDefined()) {
    std::string coord_system = config["camera-position"][0].as<std::string>();
    auto loc = config["camera-position"][1].as<std::vector<float>>();
    if (coord_system == "cartesian") {
      params.r = std::sqrt(std::pow(loc[0], 2) + std::pow(loc[1], 2) +
                           std::pow(loc[2], 2));
      params.el = std::acos(loc[2] / params.r);
      params.az = std::atan2(loc[1], loc[0]);
    } else {
      // Assume spherical coordinate system [r, inclination, azimuth]
      params.r = loc[0];
      params.el = loc[1];
      params.az = loc[2];
    }
  }

  // Color of the surface [r, b, g, a]
  if (config["color"].IsDefined()) {
    // If ignore-texture:true, this color will override any existing texture or
    // colormap.
    for (size_t i = 0; i < config["color"].size(); ++i)
      params.color[i] = config["color"][i].as<float>();
  }

  // If true, texture or any other surface colors attached to the shape will be
  // overridden.
  if (config["is-color-forced"].IsDefined())
    params.is_color_forced = config["is-color-forced"].as<bool>();

  // Camera view target location [x, y, z]
  if (config["lookat-position"].IsDefined()) {
    for (size_t i = 0; i < config["lookat-position"].size(); ++i)
      params.target[i] = config["lookat-position"][i].as<float>();
  }

  // One of X, Y, Z. This affects the up vector and how the grid is shown.
  if (config["up-axis"].IsDefined()) {
    auto up_axis_string = config["up-axis"].as<std::string>();
    if (up_axis_string == "X") {
      params.up_axis = scry::X;
    } else if (up_axis_string == "Y") {
      params.up_axis = scry::Y;
    } else {
      params.up_axis = scry::Z;
    }
  }

  // Up vector tilt angle. 0 points to up-axis.
  if (config["up-angle"].IsDefined()) {
    auto up_angle = config["up-angle"].as<float>();
    params.up_angle = up_angle;
  }

  // Image/window size [width, height]
  if (config["size"].IsDefined()) {
    params.image_width = config["size"][0].as<int>();
    params.image_height = config["size"][1].as<int>();
  } else {
    params.image_width = params.image_height = 640;
  }

  // Background color [r, g, b, a]. range is [0, 1].
  if (config["background-color"].IsDefined()) {
    for (size_t i = 0; i < config["background-color"].size(); ++i)
      params.background[i] = config["background-color"][i].as<float>();
  }

  // Near and far z clipping planes [near, far]
  if (config["clipping-planes"].IsDefined()) {
    params.near = config["clipping-planes"][0].as<float>();
    params.far = config["clipping-planes"][1].as<float>();
  }

  // Field of view in degrees
  if (config["field-of-view"].IsDefined()) {
    params.fov = config["field-of-view"].as<float>();
  }

  // Anti-aliasing level. 0 to turn off. Typically 4 or 8
  if (config["anti-aliasing"].IsDefined()) {
    params.num_msaa_samples = config["anti-aliasing"].as<int>();
  }

  // List of light properties
  if (config["lights"].IsDefined()) {
    for (size_t i = 0; i < config["lights"].size(); ++i) {
      LightProperties light;
      if (config["lights"][i]["position"].IsDefined())
        for (size_t j = 0; j < config["lights"][i]["position"].size(); ++j)
          light.light_position[j] =  
              config["lights"][i]["position"][j].as<float>();
      if (config["lights"][i]["color"].IsDefined())
        for (size_t j = 0; j < config["lights"][i]["color"][i].size(); ++j)
          light.light_color[j] = config["lights"][i]["color"][j].as<float>();
      if (config["lights"][i]["is-enabled"].IsDefined())
        light.is_enabled = config["lights"][i]["is-enabled"].as<bool>();
      if (config["lights"][i]["linear-attenuation"].IsDefined())
        light.linear_attenuation =
            config["lights"][i]["linear-attenuation"].as<float>();
      if (config["lights"][i]["constant-attenuation"].IsDefined())
        light.linear_attenuation =
            config["lights"][i]["constant-attenuation"].as<float>();
      if (config["lights"][i]["quadratic-attenuation"].IsDefined())
        light.linear_attenuation =
            config["lights"][i]["quadratic-attenuation"].as<float>();
      params.shader_params.lights.push_back(light);
    }
  } else {
    // add default light source
    LightProperties light;
    params.shader_params.lights.push_back(light);
  }

  if (config["specular-exponent"].IsDefined()) {
    params.shader_params.shininess = config["specular-exponent"].as<float>();
  }

  if (config["specular-strength"].IsDefined()) {
    params.shader_params.strength = config["specular-strength"].as<float>();
  }

  if (config["specular-strength"].IsDefined()) {
    params.shader_params.strength = config["specular-strength"].as<float>();
  }

  if (config["ambient-color"].IsDefined()) {
    for (size_t i = 0; i < config["ambient-color"].size(); ++i)
      params.shader_params.ambient[i] = config["ambient-color"][i].as<float>();
  }

  // Path to the output directory
  if (config["out-dir"].IsDefined()) {
    // Set output filename. If empty, the GUI viewer will be used.
    fs::path out_dir = config["out-dir"].as<std::string>();
    out_dir = io::FindFullPath(out_dir.string(),
                               fs::path(config_file).parent_path().string());
    params.out_filename =
        (out_dir / (fs::path(params.in_filename).stem())).string() + ".png";
  } else {
    params.out_filename = "";
  }

  // If false, it will try to resolve filename conflicts with existing files by
  // appending numbers or random string.
  if (config["can-overwrite"].IsDefined()) {
    params.can_overwrite = config["can-overwrite"].as<bool>();
  }
}
}
}
