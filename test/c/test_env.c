// use ffilesystem library from C

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef _MSC_VER
#include <direct.h>
#include <crtdbg.h>
#else
#include <unistd.h>
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

  char fpath[FS_MAX_PATH];
  char cpath[FS_MAX_PATH];

  fs_get_cwd(fpath, FS_MAX_PATH);
  printf("Fortran: current working dir %s\n", fpath);

#ifdef _MSC_VER
  if(!_getcwd(cpath, FS_MAX_PATH))
    return EXIT_FAILURE;
#else
  if(!getcwd(cpath, FS_MAX_PATH))
    return EXIT_FAILURE;
#endif

  fs_normal(cpath, cpath, FS_MAX_PATH);

  if (strcmp(fpath, cpath) != 0) {
    fprintf(stderr, "C cwd %s != Fortran cwd %s\n", cpath, fpath);
    return EXIT_FAILURE;
  }

  printf("OK: C environment\n");

  return EXIT_SUCCESS;
}
