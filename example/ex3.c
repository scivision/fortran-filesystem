// use ffilesystem library from C

#include <stdio.h>

#include "filesystem.h"

int main(void) {

  char d[MAXP];

  get_cwd(d);
  printf("current working dir %s\n", d);

  get_homedir(d);
  printf("home dir %s\n", d);

  expanduser("~", d);
  printf("expanduser('~') %s\n", d);

  return 0;
}
