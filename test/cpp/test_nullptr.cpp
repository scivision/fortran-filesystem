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

    if(fs_normal(s, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_normal(O, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_normal(s, nullptr, 0) != 0)
      return EXIT_FAILURE;

    if(fs_file_name(s, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_file_name(O, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_file_name(s, nullptr, 0) != 0)
      return EXIT_FAILURE;

    if(fs_stem(s, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_stem(O, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_stem(s, nullptr, 0) != 0)
      return EXIT_FAILURE;

    if(fs_join(s, s, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_join(O, O, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_join(s, s, nullptr, 0) != 0)
      return EXIT_FAILURE;

    if(fs_parent(s, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_parent(O, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_parent(s, nullptr, 0) != 0)
      return EXIT_FAILURE;

    if(fs_suffix(s, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_suffix(O, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_suffix(s, nullptr, 0) != 0)
      return EXIT_FAILURE;

    if(fs_with_suffix(s, s, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_with_suffix(O, O, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_with_suffix(s, s, nullptr, 0) != 0)
      return EXIT_FAILURE;

    if(fs_is_char_device(s))
      return EXIT_FAILURE;
    if(fs_is_char_device(O))
      return EXIT_FAILURE;

    if(fs_is_reserved(s))
      return EXIT_FAILURE;
    if(fs_is_reserved(O))
      return EXIT_FAILURE;

    if(fs_is_symlink(s))
      return EXIT_FAILURE;
    if(fs_is_symlink(O))
      return EXIT_FAILURE;

    if(fs_create_symlink(s, s) != 1)
      return EXIT_FAILURE;
    if(fs_create_symlink(O, O) != 1)
      return EXIT_FAILURE;

    if(fs_create_directories(s) != 1)
      return EXIT_FAILURE;
    if(fs_create_directories(O) != 1)
      return EXIT_FAILURE;

    if(fs_root(s, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_root(O, p, MAXP) != 0)
      return EXIT_FAILURE;

    if(fs_exists(s))
      return EXIT_FAILURE;
    if(fs_exists(O))
      return EXIT_FAILURE;

    if(fs_is_absolute(s))
      return EXIT_FAILURE;
    if(fs_is_absolute(O))
      return EXIT_FAILURE;

    if(fs_is_dir(s))
      return EXIT_FAILURE;
    if(fs_is_dir(O))
      return EXIT_FAILURE;

    if(fs_is_exe(s))
      return EXIT_FAILURE;
    if(fs_is_exe(O))
      return EXIT_FAILURE;

    if(fs_is_file(s))
      return EXIT_FAILURE;
    if(fs_is_file(O))
      return EXIT_FAILURE;

    if(fs_remove(s))
      return EXIT_FAILURE;
    if(fs_remove(O))
      return EXIT_FAILURE;

    if(fs_canonical(s, false, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_canonical(O, false, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_canonical(s, false, nullptr, 0) != 0)
      return EXIT_FAILURE;

    if(fs_equivalent(s, s))
      return EXIT_FAILURE;
    if(fs_equivalent(O, O))
      return EXIT_FAILURE;

    if(fs_expanduser(s, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_expanduser(O, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_expanduser(s, nullptr, 0) != 0)
      return EXIT_FAILURE;

    if(fs_copy_file(s, s, false) == 0)
      return EXIT_FAILURE;
    if(fs_copy_file(O, O, false) == 0)
      return EXIT_FAILURE;

    if(fs_relative_to(s, s, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_relative_to(O, O, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_relative_to(s, s, nullptr, 0) != 0)
      return EXIT_FAILURE;

    if(fs_touch(s))
      return EXIT_FAILURE;
    if(fs_touch(O))
      return EXIT_FAILURE;

    if(fs_get_tempdir(s, 0) != 0)
      return EXIT_FAILURE;
    if(fs_get_tempdir(O, 1) != 0)
      return EXIT_FAILURE;

    if(fs_file_size(s) != 0)
      return EXIT_FAILURE;
    if(fs_file_size(O) != 0)
      return EXIT_FAILURE;

    if(fs_space_available(s) != 0)
      return EXIT_FAILURE;
    if(fs_space_available(O) != 0)
      return EXIT_FAILURE;

    if(fs_get_cwd(s, 0) != 0)
      return EXIT_FAILURE;
    if(fs_get_cwd(O, 1) != 0)
      return EXIT_FAILURE;

    if(fs_get_homedir(s, 0) != 0)
      return EXIT_FAILURE;
    if(fs_get_homedir(O, 1) != 0)
      return EXIT_FAILURE;

    if(fs_exe_dir(s, 0) != 0)
      return EXIT_FAILURE;
    if(fs_exe_dir(O, 1) != 0)
      return EXIT_FAILURE;

    if(fs_lib_dir(s, 0) != 0)
      return EXIT_FAILURE;
    if(fs_lib_dir(O, 1) != 0)
      return EXIT_FAILURE;

    if(fs_chmod_exe(s))
      return EXIT_FAILURE;
    if(fs_chmod_exe(O))
      return EXIT_FAILURE;

    if(fs_chmod_no_exe(s))
      return EXIT_FAILURE;
    if(fs_chmod_no_exe(O))
      return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
