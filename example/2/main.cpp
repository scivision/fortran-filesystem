// use ffilesystem library from C++

#include <iostream>
#include <string>
#include <exception>
#include <cstdlib>

#include "ffilesystem.h"

int main() {

  std::cout << "current working dir " << fs_get_cwd() << "\n";

  std::string h;
  h = fs_get_homedir();
  if (h.empty())
    throw std::runtime_error("home dir not found");

  std::cout << "home dir " << h << "\n";

  h = fs_expanduser("~");
  if (h.empty())
    throw std::runtime_error("home dir not found");

  std::cout << "expanduser('~') " << h << "\n";

  return EXIT_SUCCESS;
}
