// use ffilesystem library from C++

#include <iostream>
#include <cstring>

#ifdef _MSC_VER
#include <direct.h>
#else
#include <unistd.h>
#endif

#include "filesystem.h"

int main() {

  char fpath[MAXP];
  char cpath[MAXP];

  get_cwd(fpath);
  std::cout << "Fortran: current working dir " << fpath << std::endl;

#ifdef _MSC_VER
    _getcwd(cpath, MAXP);
#else
    getcwd(cpath, MAXP);
#endif

  std::cout << "C++: current working dir " << cpath << std::endl;

  return EXIT_SUCCESS;
}
