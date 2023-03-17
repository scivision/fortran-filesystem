#include <iostream>
#include <cstdlib>
#include <filesystem>

#ifndef __cpp_lib_filesystem
#error "C++17 filesystem not supported"
#endif

namespace fs = std::filesystem;

#ifdef __MINGW32__
#include "windows_read_symlink.c"
#endif


int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <path>\n";
    return EXIT_FAILURE;
  }

  fs::path p = fs::canonical(argv[1]);
#ifdef __MINGW32__
  char buf[_MAX_PATH];
  size_t L = fs_win32_read_symlink(p.string().c_str(), buf, _MAX_PATH);
  if (!L) {
    std::cerr << "Error: " << p.string() << " failed read_symlink\n";
    return EXIT_FAILURE;
  }

  p = fs::path(buf);
#endif

  std::cout << p.generic_string() << '\n';

  return EXIT_SUCCESS;

}
