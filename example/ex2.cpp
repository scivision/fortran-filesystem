// use ffilesystem library from C++

#include <iostream>

#include "ffilesystem.h"

int main() {

  char d[MAXP];

  get_cwd(d, MAXP);
  std::cout << "current working dir " << d << std::endl;

  get_homedir(d, MAXP);
  std::cout << "home dir " << d << std::endl;

  expanduser("~", d, MAXP);
  std::cout << "expanduser('~') " << d << std::endl;

  return EXIT_SUCCESS;
}
