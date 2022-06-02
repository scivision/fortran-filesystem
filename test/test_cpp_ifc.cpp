// use ffilesystem library from C++

#include <iostream>

#if __has_include(<filesystem>)
#include <filesystem>
namespace fs = std::filesystem;
#else
#error "No C++ filesystem support"
#endif

#include "filesystem.h"

int main() {

  char fpath[4096];

  get_cwd(fpath);
  std::cout << "Fortran: current working dir " << fpath << std::endl;

  auto cwd = fs::current_path();
  std::cout << "C++: current working dir " << cwd << std::endl;

  return EXIT_SUCCESS;
}
