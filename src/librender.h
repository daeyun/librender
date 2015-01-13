/**
 * @file librender.h
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright librender is free software released under the BSD 2-Clause
 * license.
 */
#pragma once

#include <string>
#include "graphics.h"

#ifndef VERSION
#define VERSION "unknown version"
#endif

namespace librender {
namespace config {

int InitFromMainArgs(int argc, char* argv[],
                     std::vector<RenderParams>& all_params);
void InitFromFile(const std::string& filename, RenderParams& params);
}
}
