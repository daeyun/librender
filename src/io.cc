/**
 * @file io.cc
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright Scry is free software released under the BSD 2-Clause license.
 */
#include "io.h"

#include <iomanip>
#include <pwd.h>
#include <random>
#include "third_party/lodepng/lodepng.h"
#include "config.h"

namespace po = boost::program_options;
namespace fs = boost::filesystem;
namespace scry {
namespace io {

/**
 * @brief Cross-platform user directory. Check $HOME first on all platforms.
 * @return A path string to user directory.
 */
std::string FindUserDir() {
  const char* homedir = getenv("HOME");
  if (homedir) {
    return std::string(homedir);
  }

#if BOOST_OS_WINDOWS
  if ((homedir = getenv("USERPROFILE"))) {
    return std::string(homedir);
  }

  const char* hdrive = getenv("HOMEDRIVE");
  const char* hpath = getenv("HOMEPATH");
  if (hdrive && hpath) {
    return std::string(hdrive) + hpath;
  }
#else
  if ((homedir = getpwuid(getuid())->pw_dir)) {
    return std::string(homedir);
  }
#endif

  throw std::runtime_error("Unable to find home directory.");
}

/**
 * @brief Replace "~" with the user's home directory. Do nothing if \a path does
 *        not start with a tilde.
 * @param path Path starting with ~.
 * @return Absolute path with the user's home directory in place of ~.
 */
std::string ExpandUserPath(std::string path) {
  if (!path.empty() && path[0] == '~') {
    if (!(path.size() == 1 or path[1] == '/')) {
      throw std::runtime_error("Invalid user path: " + path);
    }
    path.replace(0, 1, FindUserDir());
  }
  return path;
}

/**
 * @brief Find absolute, canonical, expanded path.
 *        e.g. ~/daeyun/.. to /home
 *             . to /home/daeyun/path/to/current/dir
 * @param path Relative or absolute path containing ~, .. and .
 * @return Absolute path
 */
std::string FindFullPath(const std::string& path, const std::string& base) {
  if (base.empty())
    return fs::canonical(ExpandUserPath(path)).string();
  return fs::absolute(ExpandUserPath(path), base).string();
}

/**
 * @brief Insert a string between filename and extension
 *        e.g. /path/to/file.txt to /path/to/filestr.txt
 *        e.g. /path/to/file to /path/to/filestr
 *        e.g. file.txt to filestr.txt
 * @param path Path containing extention. If no extension is found, \a str is
 *             simply appended to \a path.
 * @param str
 * @return
 */
std::string AppendToFilename(const std::string& path, const std::string& str) {
  fs::path p(path);
  fs::path dir = p;
  dir.remove_filename();
  std::string rawname = p.stem().string();
  if (rawname == ".") rawname = "";
  // ext includes the dot (e.g. ".png"); empty if not found.
  std::string ext = p.extension().string();
  std::string new_filename = rawname + str + ext;
  return (dir / new_filename).string();
}

/**
 * @brief Generate a random lowercase alphanumeric string (i.e. [0-9a-z]*) of
 *        length \a len.
 * @param len Length
 * @return Random string
 */
std::string RandomString(size_t len) {
  if (len == 0) return "";

  static const std::string kAlphaNums =
      "0123456789"
      "abcdefghijklmnopqrstuvwxyz";

  static std::mt19937 rg(
      std::chrono::system_clock::now().time_since_epoch().count());
  static std::uniform_int_distribution<> pick(0, kAlphaNums.size() - 1);
  static auto randchar = []() -> char { return kAlphaNums[pick(rg)]; };

  std::string str(len, 0);
  std::generate_n(str.begin(), len, randchar);

  return str;
}

/**
 * @brief e.g. "/path/to/" to "/path/to",
 *             "" to "",
 *             "/" to "/"
 * @param[in,out] path The new path is stored here.
 * @see AppendSepToPath
 */
void RemoveTrailingSep(std::string& path) {
  const char kSeparatorChar = fs::path::preferred_separator;
  if (path.size() > 1 && path.back() == kSeparatorChar) {
    path.pop_back();
  }
}

/**
 * @brief e.g. "/path/to" to "/path/to/",
 *             "" to ""
 *             "/" to "/"
 * @param[in,out] path The new path is stored here.
 * @see RemoveTrailingSep
 */
void AppendSepToPath(std::string& path) {
  const char kSeparatorChar = fs::path::preferred_separator;
  if (!path.empty() && path.back() != kSeparatorChar) {
    path += kSeparatorChar;
  }
}

/**
 * @brief Find a non-existing filename. If \a filename does not exist or is
 *        empty, do nothing. e.g. If /path/to.txt exists, try /path/to.001.txt
 *        and so on, up to \a max_num. After that, try appending random strings
 *        if \a use_random is true.
 * @param[in,out] filename New filename is stored here.
 * @param max_num Default: 20
 * @param use_random Default: true
 * @return True if there was a conflict and a new name was found. False if \a
 *         filename is empty or not a conflicting name.
 */
bool ResolveFilenameConflict(std::string& filename, int max_num,
                             bool use_random) {
  if (filename.empty() || !fs::exists(filename)) return false;

  const char kSeparatorChar = fs::path::preferred_separator;
  bool has_trailing_sep = filename.back() == kSeparatorChar;
  if (has_trailing_sep) RemoveTrailingSep(filename);

  // max(3, max number of digits)
  int min_suffix_len = std::max(3, [](int i) -> int {
                                     if (i == 0) return 1;
                                     return (int)std::log10(i) + 1;
                                   }(max_num));
  const char kFillChar = '0';
  const char kBeginningChar = '.';

  std::string conflicted_name = filename;
  for (int i = 1; i <= max_num; i++) {
    std::stringstream ss;
    ss << std::setw(min_suffix_len) << std::setfill(kFillChar) << i;
    std::string suffix = kBeginningChar + ss.str();
    std::string newname = AppendToFilename(conflicted_name, suffix);
    if (!fs::exists(newname)) {
      if (has_trailing_sep) AppendSepToPath(newname);
      filename = newname;
      return true;
    }
  }

  if (use_random) {
    const int kNumRandRetry = 4;
    const int kRandStringLength = 4;

    for (int i = 0; i < kNumRandRetry; i++) {
      std::string rand_suffix =
          kBeginningChar + RandomString(kRandStringLength);

      std::string newname = AppendToFilename(conflicted_name, rand_suffix);
      if (!fs::exists(newname)) {
        if (has_trailing_sep) AppendSepToPath(newname);
        filename = newname;
        return true;
      }
    }
  }

  throw std::runtime_error("Unable to find a non-existing filename.");
}

/**
 * @brief Write raw pixel values in PNG format.
 * @param filename Path to the target file.
 * @param data Raw pixel values from the OpenGL framebuffer.
 * @param w,h Size of the image.
 * @param allow_overwrite
 */
void SaveAsPNG(std::string filename, const uint8_t* data, int w, int h,
               bool allow_overwrite) {
  if (!allow_overwrite) ResolveFilenameConflict(filename);
  fs::create_directories(fs::path(filename).parent_path());
  if (config::is_verbose) std::cout << "Saving as " << filename << std::endl;
  lodepng::encode(filename, data, w, h);
}

/**
 * @brief Recursively search upwards for a file.
 * @param dir
 * @param filename
 * @return Path to the found file. Empty string if not found.
 */
std::string UpSearch(const fs::path& dir, const fs::path& filename) {
  fs::path path = dir / filename;
  if (fs::exists(path)) return path.string();
  if (!dir.has_parent_path()) return "";
  return UpSearch(dir.parent_path(), filename);
}
}
}
