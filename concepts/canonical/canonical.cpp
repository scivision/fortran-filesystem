#include <iostream>
#include <cstdlib>
#include <filesystem>

#ifndef __cpp_lib_filesystem
#error "C++17 filesystem not supported"
#endif

namespace fs = std::filesystem;


int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << "Usage: canonical <path>\n";
    return EXIT_FAILURE;
  }

  fs::path p = fs::weakly_canonical(argv[1]);

  std::cout << "weakly_canonical(" << argv[1] << ") = " << p << '\n';

  return EXIT_SUCCESS;

}
