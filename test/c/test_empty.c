// verify functions handle empty input OK

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#ifdef _MSC_VER
#include <crtdbg.h>
#endif

#include "ffilesystem.h"


int main(void){

#ifdef _MSC_VER
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
#endif

    char O[1], p[FS_MAX_PATH];

    O[0] = '\0';

    fs_as_posix(O);
    printf("PASS: as_posix\n");

    fs_as_windows(O);
    printf("PASS: as_windows\n");

    if(fs_normal(O, p, FS_MAX_PATH) != 0)
      return EXIT_FAILURE;
    printf("PASS: normal\n");

    if(fs_file_name(O, p, FS_MAX_PATH) != 0)
      return EXIT_FAILURE;

    if(fs_stem(O, p, FS_MAX_PATH) != 0)
      return EXIT_FAILURE;
    printf("PASS: stem\n");

    if(fs_join(O, O, p, FS_MAX_PATH) != 0)
      return EXIT_FAILURE;

    if(fs_parent(O, p, FS_MAX_PATH) != 0)
      return EXIT_FAILURE;

    if(fs_suffix(O, p, FS_MAX_PATH) != 0)
      return EXIT_FAILURE;

    if(fs_with_suffix(O, O, p, FS_MAX_PATH) != 0)
      return EXIT_FAILURE;

    if(fs_is_char_device(O))
      return EXIT_FAILURE;

    if(fs_is_reserved(O))
      return EXIT_FAILURE;

    if(fs_is_symlink(O))
      return EXIT_FAILURE;
    printf("PASS: is_symlink\n");

    if(fs_create_symlink(O, O) == 0)
      return EXIT_FAILURE;
    printf("PASS: create_symlink\n");

    if(fs_create_directories(O) != 1)
      return EXIT_FAILURE;
    printf("PASS: create_directories\n");

    if(fs_root(O, p, FS_MAX_PATH) != 0)
      return EXIT_FAILURE;
    printf("PASS: root\n");

    if(fs_exists(O))
      return EXIT_FAILURE;

    if(fs_is_absolute(O))
      return EXIT_FAILURE;
    printf("PASS: is_absolute\n");

    if(fs_is_dir(O))
      return EXIT_FAILURE;
    printf("PASS: is_dir('')\n");

    if(fs_is_exe(O))
      return EXIT_FAILURE;
    printf("PASS: is_exe\n");

    if(fs_is_file(O))
      return EXIT_FAILURE;
    printf("PASS: is_file\n");

    if(fs_remove(O))
      return EXIT_FAILURE;
    printf("PASS: remove\n");

    if(fs_canonical(O, false, p, FS_MAX_PATH) != 0)
      return EXIT_FAILURE;
    printf("PASS: canonical\n");

    if(fs_equivalent(O, O))
      return EXIT_FAILURE;
    printf("PASS: equivalent\n");

    if(fs_expanduser(O, p, FS_MAX_PATH) != 0)
      return EXIT_FAILURE;
    printf("PASS: expanduser\n");

    if(fs_copy_file(O, O, false) == 0)
      return EXIT_FAILURE;

    if(fs_relative_to(O, O, p, FS_MAX_PATH) != 0)
      return EXIT_FAILURE;
    printf("PASS: relative_to\n");

    if(fs_touch(O))
      return EXIT_FAILURE;
    printf("PASS: touch\n");

    if(fs_get_tempdir(O, 1) != 0)
      return EXIT_FAILURE;
    printf("PASS: get_tempdir\n");

    if(fs_file_size(O) != 0)
      return EXIT_FAILURE;

#ifndef _WIN32
    if(fs_space_available(O) != 0)
      return EXIT_FAILURE;
#endif

    if(fs_get_cwd(O, 1) != 0)
      return EXIT_FAILURE;
    printf("PASS: get_cwd\n");

    if(fs_get_homedir(O, 1) != 0)
      return EXIT_FAILURE;
    printf("PASS: get_homedir\n");

    if(!fs_is_bsd() && fs_exe_dir(O, 1) != 0)
      return EXIT_FAILURE;

    if(fs_lib_dir(O, 1) != 0)
      return EXIT_FAILURE;

    if(fs_chmod_exe(O, true))
      return EXIT_FAILURE;

    printf("OK: test_c_empty\n");

    return EXIT_SUCCESS;
}
