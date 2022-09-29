// use ffilesystem library from C

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef _MSC_VER
#include <direct.h>
#else
#include <unistd.h>
#endif

#include "ffilesystem.h"

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

  normal(cpath, cpath);

  if (strcmp(fpath, cpath) != 0) {
    fprintf(stderr, "C cwd %s != Fortran cwd %s\n", cpath, fpath);
    return EXIT_FAILURE;
  }

  return 0;
}
