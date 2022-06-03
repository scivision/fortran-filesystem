// use ffilesystem library from C++

#include <iostream>
#include <cstring>

#ifdef _MSC_VER
#include <direct.h>
#define N _MAX_PATH
#else
#include <limits.h>
#include <unistd.h>
#define N PATH_MAX
#endif

#include "filesystem.h"

int main() {

  char fpath[4096];
  char cpath[N];

  get_cwd(fpath);
  std::cout << "Fortran: current working dir " << fpath << std::endl;

#ifdef _MSC_VER
    _getcwd(cpath, N);
#else
    getcwd(cpath, N);
#endif

  std::cout << "C++: current working dir " << cpath << std::endl;

  return EXIT_SUCCESS;
}
