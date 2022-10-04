// use ffilesystem library from C++

#include <iostream>

#include "ffilesystem.h"

int main() {

  char d[MAXP];

  fs_get_cwd(d, MAXP);
  std::cout << "current working dir " << d << std::endl;

  fs_get_homedir(d, MAXP);
  std::cout << "home dir " << d << std::endl;

  fs_expanduser("~", d, MAXP);
  std::cout << "expanduser('~') " << d << std::endl;

  return EXIT_SUCCESS;
}
