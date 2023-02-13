// use ffilesystem library from C++

#include <iostream>
#include <cstdlib>
#include <cstring>

#ifdef _MSC_VER
#include <direct.h>
#else
#include <unistd.h>
#endif

#include "ffilesystem.h"

int main() {

  char fpath[MAXP];
  char cpath[MAXP];

  fs_get_cwd(fpath, MAXP);
  std::cout << "Fortran: current working dir " << fpath << std::endl;

#ifdef _MSC_VER
    if(_getcwd(cpath, MAXP)  == nullptr)
      return EXIT_FAILURE;
#else
    if(getcwd(cpath, MAXP) == nullptr)
      return EXIT_FAILURE;
#endif

  fs_normal(cpath, cpath, MAXP);

  if (std::strcmp(fpath, cpath) != 0) {
    fprintf(stderr, "C cwd %s != Fortran cwd %s\n", cpath, fpath);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
