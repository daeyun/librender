#ifndef MAIN_H_
#define MAIN_H_

#include <string>
#include <vector>
#include <armadillo>

#ifndef VERSION
#define VERSION "unknown version"
#endif

// Constants
const std::string kConfigFileName = ".scryrc";

const std::string kAppName = "Scry";
const std::string kAppDesc = "Scry is a tool for previewing mesh files.";

// Structs
typedef struct {
    std::string config_file;
    std::vector<std::string> mesh_files;
    int resolution;
} options_t;

// Functions
int parse_options(int argc, char** argv, options_t& options);

#endif  // MAIN_H_
