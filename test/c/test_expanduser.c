#include <stdio.h>
#include <stdlib.h>
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

  char r[FS_MAX_PATH];
  char h[FS_MAX_PATH];

  size_t L = fs_expanduser("", r, FS_MAX_PATH);

  if(L != 0) {
    fprintf(stderr, "expanduser('') != ''");
    return EXIT_FAILURE;
  }

  L = fs_expanduser(".", r, FS_MAX_PATH);
  if (L != 1 || strcmp(r, ".") != 0){
    fprintf(stderr, "expanduser dot failed: %s\n", r);
    return EXIT_FAILURE;
  }

  L = fs_expanduser("~", r, FS_MAX_PATH);
  size_t L2 = fs_get_homedir(h, FS_MAX_PATH);
  if(L != L2 || strcmp(r, h) != 0){
    fprintf(stderr, "expanduser ~ failed: %s %s\n", r, h);
    return EXIT_FAILURE;
  }
  printf("~: %s\n", h);

  L = fs_expanduser("~/", r, FS_MAX_PATH);
  L2 = fs_get_homedir(h, FS_MAX_PATH);
  if(L != L2 || strcmp(r, h) != 0){
    fprintf(stderr, "expanduser ~/ failed: %s %s\n", r, h);
    return EXIT_FAILURE;
  }
  printf("~/: %s\n", h);

  L2 = fs_expanduser("~//", h, FS_MAX_PATH);
  if(L != L2 || strcmp(r, h) != 0){
    fprintf(stderr, "expanduser ~// failed: %s %s\n", r, h);
    return EXIT_FAILURE;
  }

  printf("OK: filesystem_C: expanduser:  %s\n", h);

  return EXIT_SUCCESS;
}
