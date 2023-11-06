#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ffilesystem.h"


int main(void)
{

char out[FS_MAX_PATH];

// -- home directory
size_t L0 = fs_canonical("~", false, out, FS_MAX_PATH);

if(strcmp(out, "~") == 0){
  fprintf(stderr, "canonical(~) did not expanduser: %s\n", out);
  return EXIT_FAILURE;
}
printf("OK: home dir = %s\n", out);

size_t L1 = fs_parent(out, out, FS_MAX_PATH);
if (L1 >= L0)
  return EXIT_FAILURE;
printf("OK: parent home = %s\n", out);

// -- relative dir
const char* par = "~/..";

size_t L2 = fs_canonical(par, false, out, FS_MAX_PATH);

if (L2 != L1){
  fprintf(stderr, "ERROR:canonical:relative: up dir not canonicalized: ~/.. => %s\n", out);
  return EXIT_FAILURE;
}
printf("OK: canon_rel_up = %s\n", out);

// -- relative file
if(fs_is_cygwin())
  // Cygwin can't handle non-existing canonical paths
  return EXIT_SUCCESS;

const char* file = "~/../not-exist.txt";

size_t L = fs_canonical(file, false, out, FS_MAX_PATH);
if(L == 0) {
  fprintf(stderr, "ERROR: relative file did not resolve: %s\n", file);
  return EXIT_FAILURE;
}

if (L - L2 != 13 + 1){
  fprintf(stderr, "ERROR relative file was not canonicalized: %s %zu %zu\n", file, L0, L);
  return EXIT_FAILURE;
}

return EXIT_SUCCESS;
}
