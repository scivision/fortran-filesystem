// use ffilesystem library from C

#include <stdio.h>

#include "ffilesystem.h"

int main(void) {

  char d[FS_MAX_PATH];

  fs_get_cwd(d, FS_MAX_PATH);
  printf("current working dir %s\n", d);

  fs_get_homedir(d, FS_MAX_PATH);
  printf("home dir %s\n", d);

  fs_expanduser("~", d, FS_MAX_PATH);
  printf("expanduser('~') %s\n", d);

  return 0;
}
