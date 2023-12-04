#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
#include <crtdbg.h>
#endif

#include "ffilesystem.h"


int test_lib_path(char* argv[]){

  char binpath[FS_MAX_PATH], bindir[FS_MAX_PATH], p[FS_MAX_PATH];

  int shared = atoi(argv[1]);

  size_t L = fs_lib_path(binpath, FS_MAX_PATH);
  size_t L2 = fs_lib_dir(bindir, FS_MAX_PATH);

  if(!shared) {
    if (L != 0 || L2 != 0) {
      fprintf(stderr, "ERROR:test_binpath: lib_path and lib_dir should be empty length 0: %s %zu\n", binpath, L);
      return 1;
    }
    fprintf(stderr, "SKIP: lib_path: feature not available\n");
    return 0;
  }

  if(!L){
    fprintf(stderr, "ERROR:test_binpath: lib_path should be non-empty: %s %zu\n", binpath, L);
    return 1;
  }

  if(!strstr(binpath, argv[2])){
    fprintf(stderr, "ERROR:test_binpath: lib_path not found correctly: %s does not contain %s\n", binpath, argv[3]);
    return 1;
  }

  printf("OK: lib_path: %s\n", binpath);

  fs_parent(binpath, p, FS_MAX_PATH);

  printf("parent(lib_path): %s\n", p);

  if(!L2){
    if(fs_is_cygwin()){
      fprintf(stderr, "SKIP: lib_dir: feature not available on cygwin\n");
      return 0;
    }
    fprintf(stderr, "ERROR:test_binpath: lib_dir should be non-empty: %s %zu\n", bindir, L2);
    return 1;
  }

  if(!fs_equivalent(bindir, p)){
    fprintf(stderr, "ERROR:test_binpath_c: lib_dir and parent(lib_path) should be equivalent: %s %s\n", bindir, p);
    return 1;
  }

  printf("OK: lib_dir: %s\n", bindir);

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

  if (argc < 3) {
    fprintf(stderr, "ERROR: test_binpath_c: not enough arguments\n");
    return 1;
  }

  return test_lib_path(argv);

}
