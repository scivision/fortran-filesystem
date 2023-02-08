// verify functions handle nullptr input OK
// Fortran tests already verify empty string.

#include <cstdlib>
#include <iostream>

#include <ffilesystem.h>



int main(){
    char *s = nullptr;
    char O[1];
    char p[MAXP];

    O[0] = '\0';

    if(fs_normal(s, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_normal(O, p, MAXP) != 0)
      return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
