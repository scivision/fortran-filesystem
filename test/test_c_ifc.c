// use ffilesystem library from C

#include <stdio.h>

#ifdef _MSC_VER
#include <direct.h>
#else
#include <unistd.h>
#endif

#include "filesystem.h"

#define N 4096

int main() {

  char fpath[N];
  char cpath[N];

  get_cwd(fpath);
  printf("Fortran: current working dir %s\n", fpath);

  getcwd(cpath, N);
  printf("C: current working dir %s\n", cpath);

  return 0;
}
