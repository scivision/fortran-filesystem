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

#ifdef _MSC_VER
  _getcwd(cpath, N);
#else
  getcwd(cpath, N);
#endif
  as_posix(cpath);

  if (strcmp(fpath, cpath) != 0) {
    fprintf(stderr, "C cwd %s != Fortran cwd %s\n", cpath, fpath);
    return 1;
  }

  return EXIT_SUCCESS;
}
