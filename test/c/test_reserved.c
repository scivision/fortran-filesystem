#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

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

#ifdef _WIN32
    char s[] = "NUL";
    const char ref[] = "NUL";
#else
    char s[] = "/dev/null";
    const char ref[] = "/dev/null";
#endif

    char p[FS_MAX_PATH];

    printf("Begin test_reserved\n");

    fs_normal(s, p, FS_MAX_PATH);
    if (strcmp(p, ref) != 0){
      fprintf(stderr,"FAIL: normal(%s)  %s\n", s, p);
      return EXIT_FAILURE;
    }
    printf("OK: normal(%s)\n", p);

    bool b = fs_is_absolute(s);
    if (fs_is_windows()){
      if(b) return EXIT_FAILURE;
    }
    else{
      if(!b) return EXIT_FAILURE;
    }
    printf("OK: is_absolute(%s)\n", ref);

    if(fs_is_dir(s))
      return EXIT_FAILURE;

    if(fs_is_exe(s))
      return EXIT_FAILURE;

#ifndef _WIN32

    // NOTE: do not test
    //
    // create_directories(/dev/null)
    // remove(/dev/null)
    // create_symlink()
    // chmod_exe()
    //
    // since if testing with "root" privilidges,
    // it can make the system unusable until reboot!

    if(!fs_exists(s))
      return EXIT_FAILURE;
    printf("OK: exists(%s)\n", ref);

    b = fs_is_file(s);
    if(b){
      fprintf(stderr,"FAIL: is_file(%s) %d\n", s, b);
      return EXIT_FAILURE;
    }

    if(fs_canonical(s, false, p, FS_MAX_PATH) == 0){
      fprintf(stderr,"FAIL: canonical(%s)  %s\n", s, p);
      return EXIT_FAILURE;
    }
    printf("OK: canonical(%s)\n", p);

    fs_relative_to(s, s, p, FS_MAX_PATH);
    if(strcmp(p, ".") != 0){
      fprintf(stderr,"FAIL: relative_to(%s)  %s\n", ref, p);
      return EXIT_FAILURE;
    }
#endif

    fs_expanduser(s, p, FS_MAX_PATH);
    if(strcmp(p, ref) != 0)
      return EXIT_FAILURE;

    if(fs_copy_file(s, s, false) == 0){
      fprintf(stderr,"FAIL: copy_file(%s)\n", s);
      return EXIT_FAILURE;
    }
    printf("OK: copy_file(%s)\n", s);

    if(fs_touch(s))
      return EXIT_FAILURE;
    printf("OK: touch(%s)\n", ref);

    if(fs_file_size(s) != 0)
      return EXIT_FAILURE;
    printf("OK: file_size(%s)\n", ref);

    if(fs_is_symlink(s))
      return EXIT_FAILURE;
    printf("OK: is_symlink(%s)\n", s);

    printf("PASS: test_reserved.cpp\n");

    return EXIT_SUCCESS;
}
