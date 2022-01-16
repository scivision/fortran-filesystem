// functions from C++17 filesystem
// They don't necessarily work on all platform/compiler combinations
// For example, is_symlink() doesn't work on Windows with G++17,
// but does work on Windows with Clang and Intel oneAPI.

#include <filesystem>

extern "C" bool is_symlink(const char* path) {
  return std::filesystem::is_symlink(path);
}
