// use ffilesystem library from C

#include <stdio.h>

#ifdef _MSC_VER
#include <direct.h>
#else
#include <unistd.h>
#endif

#include "filesystem.h"

int main() {

  size_t N = 4096;
  char fpath[N], cpath[N];

  get_cwd(fpath);
  printf("Fortran: current working dir %s\n", fpath);

  getcwd(cpath, N);
  printf("C: current working dir %s\n", cpath);

  return 0;
}
