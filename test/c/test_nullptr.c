// verify functions handle NULL input OK
// Fortran tests already verify empty string.

#include <stdlib.h>
#include <stdio.h>

#ifdef _MSC_VER
#include <crtdbg.h>
#endif

#include <ffilesystem.h>



int main(){

#ifdef _MSC_VER
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
#endif

    char *s = NULL;
    char O[1];
    char p[MAXP];

    O[0] = '\0';

    fs_as_posix(s);
    if (s != NULL)
      return EXIT_FAILURE;
    fs_as_windows(s);
    if (s != NULL)
      return EXIT_FAILURE;

    if(fs_normal(s, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_normal(O, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_normal(s, NULL, 0) != 0)
      return EXIT_FAILURE;
    printf("PASS: normal\n");

    if(fs_file_name(s, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_file_name(O, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_file_name(s, NULL, 0) != 0)
      return EXIT_FAILURE;

    if(fs_stem(s, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_stem(O, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_stem(s, NULL, 0) != 0)
      return EXIT_FAILURE;
    printf("PASS: stem\n");

    if(fs_join(s, s, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_join(O, O, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_join(s, s, NULL, 0) != 0)
      return EXIT_FAILURE;

    if(fs_parent(s, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_parent(O, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_parent(s, NULL, 0) != 0)
      return EXIT_FAILURE;

    if(fs_suffix(s, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_suffix(O, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_suffix(s, NULL, 0) != 0)
      return EXIT_FAILURE;

    if(fs_with_suffix(s, s, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_with_suffix(O, O, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_with_suffix(s, s, NULL, 0) != 0)
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
    printf("PASS: is_symlink\n");

    if(fs_create_symlink(s, s) == 0)
      return EXIT_FAILURE;
    if(fs_create_symlink(O, O) == 0)
      return EXIT_FAILURE;
    printf("PASS: create_symlink\n");

    if(fs_create_directories(s) != 1)
      return EXIT_FAILURE;
    if(fs_create_directories(O) != 1)
      return EXIT_FAILURE;
    printf("PASS: create_directories\n");

    if(fs_root(s, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_root(O, p, MAXP) != 0)
      return EXIT_FAILURE;
    printf("PASS: root\n");

    if(fs_exists(s))
      return EXIT_FAILURE;
    if(fs_exists(O))
      return EXIT_FAILURE;

    if(fs_is_absolute(s))
      return EXIT_FAILURE;
    if(fs_is_absolute(O))
      return EXIT_FAILURE;
    printf("PASS: is_absolute\n");

    if(fs_is_dir(s))
      return EXIT_FAILURE;
    if(fs_is_dir(O))
      return EXIT_FAILURE;

    if(fs_is_exe(s))
      return EXIT_FAILURE;
    if(fs_is_exe(O))
      return EXIT_FAILURE;
    printf("PASS: is_exe\n");

    if(fs_is_file(s))
      return EXIT_FAILURE;
    if(fs_is_file(O))
      return EXIT_FAILURE;
    printf("PASS: is_file\n");

    if(fs_remove(s))
      return EXIT_FAILURE;
    if(fs_remove(O))
      return EXIT_FAILURE;
    printf("PASS: remove\n");

    if(fs_canonical(s, false, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_canonical(O, false, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_canonical(s, false, NULL, 0) != 0)
      return EXIT_FAILURE;
    printf("PASS: canonical\n");

    if(fs_equivalent(s, s))
      return EXIT_FAILURE;
    if(fs_equivalent(O, O))
      return EXIT_FAILURE;

    if(fs_expanduser(s, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_expanduser(O, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_expanduser(s, NULL, 0) != 0)
      return EXIT_FAILURE;
    printf("PASS: expanduser\n");

    if(fs_copy_file(s, s, false) == 0)
      return EXIT_FAILURE;
    if(fs_copy_file(O, O, false) == 0)
      return EXIT_FAILURE;

    if(fs_relative_to(s, s, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_relative_to(O, O, p, MAXP) != 0)
      return EXIT_FAILURE;
    if(fs_relative_to(s, s, NULL, 0) != 0)
      return EXIT_FAILURE;
    printf("PASS: relative_to\n");

    if(fs_touch(s))
      return EXIT_FAILURE;
    if(fs_touch(O))
      return EXIT_FAILURE;
    printf("PASS: touch\n");

    if(fs_get_tempdir(s, 0) != 0)
      return EXIT_FAILURE;
    if(fs_get_tempdir(O, 1) != 0)
      return EXIT_FAILURE;
    printf("PASS: get_tempdir\n");

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
    printf("PASS: get_cwd\n");

    if(fs_get_homedir(s, 0) != 0)
      return EXIT_FAILURE;
    if(fs_get_homedir(O, 1) != 0)
      return EXIT_FAILURE;
    printf("PASS: get_homedir\n");

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

    printf("OK: test_c_nullptr\n");

    return EXIT_SUCCESS;
}
