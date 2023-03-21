// use ffilesystem library from C++

#include <iostream>
#include <cstdlib>

#include "ffilesystem.h"

int main() {

  std::cout << "current working dir " << fs_get_cwd() << "\n";;

  std::cout << "home dir " << fs_get_homedir() << "\n";;

  std::cout << "expanduser('~') " << fs_expanduser("~") << "\n";;

  return EXIT_SUCCESS;
}
