// functions from C++17 filesystem

#include <cstring>
#include <filesystem>

namespace fs = std::filesystem;

extern "C" bool is_symlink(const char* path) {
  return fs::is_symlink(path);
}

extern "C" void create_symlink(const char* target, const char* link) {
  fs::create_symlink(target, link);
}

extern "C" void create_directory_symlink(const char* target, const char* link) {
  fs::create_directory_symlink(target, link);
}

extern "C" bool exists(const char* path) {
  return fs::exists(path);
}

extern "C" bool fs_remove(const char* path) {
  return fs::remove(path);
}


extern "C" bool copy_file(const char* source, const char* destination, bool overwrite) {

  auto opt = fs::copy_options::none;

  if (overwrite) {

// WORKAROUND: Windows MinGW GCC 11, Intel oneAPI Linux: bug with overwrite_existing failing on overwrite
  if(fs::exists(destination)) fs::remove(destination);

  opt |= fs::copy_options::overwrite_existing;
  }

  return fs::copy_file(source, destination, opt);
}


extern "C" size_t relative_to(const char* a, const char* b, char* result) {

  auto r = fs::relative(a, b);

  strcpy(result, r.string().c_str());
  auto result_size = strlen(result);

  // std::cout << "TRACE:relative_to: " << a << " " << b << " " << r << " " << result_size << std::endl;

  return result_size;
}
