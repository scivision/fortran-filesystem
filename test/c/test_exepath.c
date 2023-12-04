#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
#include <crtdbg.h>
#endif

#include "ffilesystem.h"


int test_exe_path(char* argv[]){

  char binpath[FS_MAX_PATH], bindir[FS_MAX_PATH], p[FS_MAX_PATH];

  fs_exe_path(binpath, FS_MAX_PATH);
  if (!strstr(binpath, argv[1])) {
    fprintf(stderr, "ERROR:test_binpath: exe_path not found correctly: %s\n", binpath);
    return 1;
  }

  size_t L = fs_exe_dir(bindir, FS_MAX_PATH);
  if(L == 0){
    fprintf(stderr, "ERROR:test_binpath: exe_dir not found correctly: %s\n", bindir);
    return 1;
  }
  fs_parent(binpath, p, FS_MAX_PATH);

  if(!fs_equivalent(bindir, p)){
    fprintf(stderr, "ERROR:test_binpath: exe_dir and parent(exe_path) should be equivalent: %s %s\n", bindir, p);
    return 1;
  }

  printf("OK: exe_path: %s\n", binpath);
  printf("OK: exe_dir: %s\n", bindir);
  return 0;
}

int main(int argc, char* argv[]){

#ifdef _MSC_VER
  _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
  _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
#endif

  if (argc < 2) {
    fprintf(stderr, "ERROR: test_binpath_c: not enough arguments\n");
    return 1;
  }

  return test_exe_path(argv);

}
