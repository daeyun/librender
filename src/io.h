/**
 * @file io.h
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright Scry is free software released under the BSD 2-Clause license.
 */
#pragma once

#include <string>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
namespace fs = boost::filesystem;
namespace scry {
namespace io {

std::string FindUserDir();
std::string ExpandUserPath(std::string path);
std::string FindFullPath(const std::string& path, const std::string& base="");
std::string AppendToFilename(const std::string& path, const std::string& str);
std::string RandomString(size_t len);
void RemoveTrailingSep(std::string& path);
void AppendSepToPath(std::string& path);
bool ResolveFilenameConflict(std::string& filename, int max_num = 20,
                             bool use_random = true);
void SaveAsPNG(std::string filename, const uint8_t* data, int w, int h,
               bool allow_overwrite = true);
std::string UpSearch(const fs::path& dir, const fs::path& filename);
}
}
