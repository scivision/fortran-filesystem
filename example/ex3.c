// use ffilesystem library from C

#include <stdio.h>

#include "ffilesystem.h"

int main(void) {

  char d[MAXP];

  get_cwd(d, MAXP);
  printf("current working dir %s\n", d);

  get_homedir(d, MAXP);
  printf("home dir %s\n", d);

  expanduser("~", d, MAXP);
  printf("expanduser('~') %s\n", d);

  return 0;
}
