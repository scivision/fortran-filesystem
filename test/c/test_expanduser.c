#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ffilesystem.h"

int main(void){

  char r[MAXP];
  char h[MAXP];

  size_t L = fs_expanduser("", r, MAXP);

  if(L != 0) {
    fprintf(stderr, "expanduser('') != ''");
    return EXIT_FAILURE;
  }

  L = fs_expanduser(".", r, MAXP);
  if (L != 1 || strcmp(r, ".") != 0){
    fprintf(stderr, "expanduser dot failed: %s\n", r);
    return EXIT_FAILURE;
  }

  L = fs_expanduser("~", r, MAXP);
  size_t L2 = fs_get_homedir(h, MAXP);
  if(L != L2 || strcmp(r, h) != 0){
    fprintf(stderr, "expanduser home failed: %s %s\n", r, h);
    return EXIT_FAILURE;
  }

  L2 = fs_expanduser("~//", h, MAXP);
  if(L != L2 || strcmp(r, h) != 0){
    fprintf(stderr, "expanduser double separator failed: %s %s\n", r, h);
    return EXIT_FAILURE;
  }

  printf("OK: filesystem_C: expanduser:  %s\n", h);

  return EXIT_SUCCESS;
}
