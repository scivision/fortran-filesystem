// functions from C++17 filesystem
// They don't necessarily work on all platform/compiler combinations
// For example, is_symlink() doesn't work on Windows with G++17,
// but does work on Windows with Clang and Intel oneAPI.

#include <filesystem>

extern "C" bool is_symlink(const char* path) {
  return std::filesystem::is_symlink(path);
}

extern "C" void create_symlink(const char* target, const char* link) {
  std::filesystem::create_symlink(target, link);
}

extern "C" void create_directory_symlink(const char* target, const char* link) {
  std::filesystem::create_directory_symlink(target, link);
}
