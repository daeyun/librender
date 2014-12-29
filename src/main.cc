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
#include "scry_object.h"

#include "graphics.h"

namespace po = boost::program_options;
namespace fs = boost::filesystem;

int main(int argc, char* argv[]) {
    options_t options;
    int parse_result = parse_options(argc, argv, options);
    if (parse_result > 0) {
        return 0;
    } else if (parse_result < 0) {
        return -1;
    }

    ScryObject mesh;
    load_obj(options.mesh_files[0], mesh);

    render_object(mesh);

    //config::load_config_file(options.config_file);

    //for (std::string mesh_file : options.mesh_files) {
        //std::cout << mesh_file << std::endl;

        //ScryObject mesh;
        //try {
            //load_obj(mesh_file, mesh);
        //} catch (std::exception e) {
            //std::cerr << e.what() << std::endl;
        //}
    //}
}

int parse_options(int argc, char** argv, options_t& options) {
    // Find default config file path.
    std::string homedir = getenv("HOME");
    if (homedir.empty()) {
        homedir = getpwuid(getuid())->pw_dir;
    }
    fs::path default_rc(homedir);
    default_rc /= kConfigFileName;

    // Declare the supported options.
    po::options_description desc("Options");
    auto config_opt = po::value<std::string>()->default_value(default_rc.string());
    auto resolution_opt = po::value<int>()->default_value(640);
    auto mesh_files_opt = po::value<std::vector<std::string>>()->required();
    desc.add_options()
        ("version,v", "print version string")
        ("help", "produce help message")
        ("config-file,c", config_opt, "path to configuration file")
        ("resolution,r", resolution_opt, "resolution of output images")
        ("mesh-files", mesh_files_opt, "supported file types: .obj");

    po::positional_options_description positionalOptions;
    positionalOptions.add("mesh-files", -1);
    po::variables_map vm;

    try {
        po::store(po::command_line_parser(argc, argv)
                      .options(desc)
                      .positional(positionalOptions)
                      .run(), vm);

        if (vm.count("version")) {
            std::cout << kAppName << " " << VERSION << std::endl;
            return 1;
        }

        if (vm.count("help") || !vm.count("mesh-files")) {
            std::cout << kAppDesc << std::endl << std::endl << desc;
            return 1;
        }

        po::notify(vm);

        options.config_file = vm["config-file"].as<std::string>();
        options.mesh_files = vm["mesh-files"].as<std::vector<std::string>>();
        options.resolution = vm["resolution"].as<int>();
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl << std::endl << desc;
        return -1;
    }

    return 0;
}
