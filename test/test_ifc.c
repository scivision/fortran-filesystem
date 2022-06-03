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

int main() {

  char fpath[N];
  char cpath[N];

  get_cwd(fpath);
  printf("Fortran: current working dir %s\n", fpath);

#ifdef _MSC_VER
  _getcwd(cpath, N);
#else
  getcwd(cpath, N);
#endif

  printf("C: current working dir %s\n", cpath);

  return 0;
}
