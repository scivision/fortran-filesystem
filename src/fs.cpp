// functions from C++17 filesystem

#include <cstring>
#include <fstream>
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

extern "C" bool create_directories(const char* path) {
  return fs::create_directories(path);
}

extern "C" bool exists(const char* path) {
  return fs::exists(path);
}

extern "C" bool is_dir(const char* path) {
  if(std::strlen(path) == 0) return false;

  fs::path p(path);

#ifdef _WIN32
  if (p.root_name() == p) return true;
#endif

  return fs::is_directory(p);
}

extern "C" bool fs_remove(const char* path) {
  return fs::remove(path);
}

extern "C" size_t canonical(char* path, bool strict){
// does NOT expand tilde ~
  fs::path p;

  if(strict){
    p = fs::canonical(path);
  }
  else {
    p = fs::weakly_canonical(path);
  }

std::strcpy(path, p.string().c_str());
auto result_size = strlen(path);

return result_size;
}


extern "C" bool equivalent(const char* path1, const char* path2) {
  // check existance to avoid error if not exist

  if (!fs::exists(path1) | !fs::exists(path2)) return false;

  return fs::equivalent(path1, path2);
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

  std::strcpy(result, r.string().c_str());
  auto result_size = strlen(result);

  // std::cout << "TRACE:relative_to: " << a << " " << b << " " << r << " " << result_size << std::endl;

  return result_size;
}


extern "C" bool touch(const char* path) {

  fs::path p(path);

  if (fs::exists(p) & !fs::is_regular_file(p)) return false;

  if(!fs::is_regular_file(p)) {
    std::ofstream ost;
    ost.open(p);
    ost.close();
  }

  if (!fs::is_regular_file(p)) return false;

  fs::last_write_time(p, std::filesystem::file_time_type::clock::now());

  return true;

}


extern "C" size_t get_tempdir(char* path) {
  std::strcpy(path, fs::temp_directory_path().string().c_str());
  return strlen(path);
}
