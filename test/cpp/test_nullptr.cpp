// verify functions handle nullptr input OK
// Fortran tests already verify empty string.

#include <cstdlib>
#include <iostream>

#include <ffilesystem.h>



int main(){
    char *s = nullptr;
    char sep[2];
    char O[1];
    char p[MAXP];

    O[0] = '\0';

    if(fs_filesep(s) != 0)
      return EXIT_FAILURE;
    std::cout << "PASS: nullptr: fs_filesep" << std::endl;
    if(fs_filesep(sep) != 1)
      return EXIT_FAILURE;
    std::cout << "PASS: fs_filesep: " << sep << std::endl;

    if(fs_normal(s, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_normal(O, p, MAXP) != 0)
      return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
