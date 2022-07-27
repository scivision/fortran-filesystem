// use ffilesystem library from C

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef _MSC_VER
#include <direct.h>
#else
#include <unistd.h>
#endif

#include "filesystem.h"

int main(void) {

  char fpath[MAXP];
  char cpath[MAXP];

  get_cwd(fpath);
  printf("Fortran: current working dir %s\n", fpath);

#ifdef _MSC_VER
  _getcwd(cpath, MAXP);
#else
  getcwd(cpath, MAXP);
#endif

  printf("C: current working dir %s\n", cpath);

  return 0;
}
