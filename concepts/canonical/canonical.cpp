#include <iostream>
#include <cstdlib>
#include <filesystem>

#include "canonical.h"

namespace fs = std::filesystem;


int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <path>\n";
    return EXIT_FAILURE;
  }

  fs::path p = fs::canonical(argv[1]);
#ifdef __MINGW32__
  std::string r = fs_win32_read_symlink(p.string());
  if (r.empty()) {
    std::cerr << "Error: " << p.string() << " failed win32_read_symlink\n";
    return EXIT_FAILURE;
  }

  p = fs::path(r);
#endif

  std::cout << p.generic_string() << '\n';

  return EXIT_SUCCESS;

}
