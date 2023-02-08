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

    fs_as_posix(s);
    if (s != nullptr)
      return EXIT_FAILURE;
    fs_as_windows(s);
    if (s != nullptr)
      return EXIT_FAILURE;

    if(fs_is_absolute(s) != 0)
      return EXIT_FAILURE;
    if(fs_is_absolute(O) != 0)
      return EXIT_FAILURE;

    if(fs_normal(s, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_normal(O, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_normal(s, nullptr, 0) != 0)
      return EXIT_FAILURE;

    if(fs_expanduser(s, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_expanduser(O, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_expanduser(s, nullptr, 0) != 0)
      return EXIT_FAILURE;

    if(fs_file_name(s, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_file_name(O, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_file_name(s, nullptr, 0) != 0)
      return EXIT_FAILURE;

    if(fs_exe_dir(s, 0) != 0)
      return EXIT_FAILURE;
    if(fs_exe_dir(O, 1) != 0)
      return EXIT_FAILURE;

    if(fs_lib_dir(s, 0) != 0)
      return EXIT_FAILURE;
    if(fs_lib_dir(O, 1) != 0)
      return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
