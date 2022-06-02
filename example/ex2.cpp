// use ffilesystem library from C++

#include <iostream>

#include "filesystem.h"

int main() {

  char d[4096];

  get_cwd(d);
  std::cout << "current working dir " << d << std::endl;

  get_homedir(d);
  std::cout << "home dir " << d << std::endl;

  expanduser("~", d);
  std::cout << "expanduser('~') " << d << std::endl;

  return EXIT_SUCCESS;
}
