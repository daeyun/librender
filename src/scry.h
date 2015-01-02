/**
 * @file scry.h
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright Scry is free software released under the BSD 2-Clause license.
 */
#pragma once

#include <string>

#ifndef VERSION
#define VERSION "unknown version"
#endif

namespace scry {
namespace config {

int InitFromMainArgs(int argc, char** argv);
void InitFromFile(const std::string& filename);
}
}
