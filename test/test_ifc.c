// use ffilesystem library from C

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef _MSC_VER
#include <direct.h>
#define N _MAX_PATH
#else
#include <limits.h>
#include <unistd.h>
#define N PATH_MAX
#endif

#include "filesystem.h"

int main(void) {

  char fpath[N];
  char cpath[N];

  get_cwd(fpath);

#ifdef _MSC_VER
  _getcwd(cpath, N);
#else
  getcwd(cpath, N);
#endif

#ifdef _WIN32
  as_posix(cpath);
#endif

  printf("%s\n%s\n", fpath, cpath);

  if (strcmp(fpath, cpath) != 0) {
    fprintf(stderr, "C cwd %s != Fortran cwd %s\n", cpath, fpath);
    return 1;
  }

  return 0;
}
