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

  get_cwd(fpath);
  std::cout << "Fortran: current working dir " << fpath << std::endl;

#ifdef _MSC_VER
    _getcwd(cpath, MAXP);
#else
    getcwd(cpath, MAXP);
#endif

  normal(cpath, cpath);

  if (strcmp(fpath, cpath) != 0) {
    fprintf(stderr, "C cwd %s != Fortran cwd %s\n", cpath, fpath);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
