#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ffilesystem.h"

int main(void){

  char r[FS_MAX_PATH];
  char h[FS_MAX_PATH];

  size_t L = fs_expanduser("", r, FS_MAX_PATH);

  if(L != 0) {
    fprintf(stderr, "expanduser('') != ''");
    return EXIT_FAILURE;
  }

  L = fs_expanduser(".", r, FS_MAX_PATH);
  if (L != 1 || strcmp(r, ".") != 0){
    fprintf(stderr, "expanduser dot failed: %s\n", r);
    return EXIT_FAILURE;
  }

  L = fs_expanduser("~", r, FS_MAX_PATH);
  size_t L2 = fs_get_homedir(h, FS_MAX_PATH);
  if(L != L2 || strcmp(r, h) != 0){
    fprintf(stderr, "expanduser home failed: %s %s\n", r, h);
    return EXIT_FAILURE;
  }
  printf("homedir: %s\n", h);

  L2 = fs_expanduser("~//", h, FS_MAX_PATH);
  if(L != L2 || strcmp(r, h) != 0){
    fprintf(stderr, "expanduser double separator failed: %s %s\n", r, h);
    return EXIT_FAILURE;
  }

  printf("OK: filesystem_C: expanduser:  %s\n", h);

  return EXIT_SUCCESS;
}
