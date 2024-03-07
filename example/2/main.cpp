// use ffilesystem library from C++

#include <iostream>
#include <string>
#include <exception>
#include <cstdlib>

#include "ffilesystem.h"

int main() {

  std::cout << "current working dir " << Ffs::get_cwd() << "\n";

  std::string h;
  h = Ffs::get_homedir();
  if (h.empty())
    throw std::runtime_error("home dir not found");

  std::cout << "home dir " << h << "\n";

  h = Ffs::expanduser("~");
  if (h.empty())
    throw std::runtime_error("home dir not found");

  std::cout << "expanduser('~') " << h << "\n";

  return EXIT_SUCCESS;
}
