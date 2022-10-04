#include "ffilesystem.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int test_exe_path(void){

  char binpath[MAXP];

  fs_exe_path(binpath, MAXP);
  if (!strstr(binpath, "test_binpath")) {
    fprintf(stderr, "ERROR:test_binpath: exe_path not found correctly: %s\n", binpath);
    return 1;
  }

  printf("OK: exe_path: %s\n", binpath);
  return 0;
}

int test_lib_path(int argc, char* argv[]){

  char binpath[MAXP];

  if(argc != 2){
    fprintf(stderr, "need argument 0 for static or 1 for shared.  Got: %s\n", argv[1]);
    return 1;
  }

  int shared = atoi(argv[1]);

  size_t L = fs_lib_path(binpath, MAXP);

  if(!shared) {
    if (L != 0){
      fprintf(stderr, "ERROR:test_binpath: lib_path should be empty length 0: %s %ju\n", binpath, L);
      return 1;
    }
    printf("SKIPPED: lib_path: due to static library\n");
    return 0;
  }

#ifdef __APPLE__
#define name "ffilesystem.dylib"
#elif defined(_WIN32)
#define name "ffilesystem.dll"
#else
#define name "libffilesystem.so"
#endif

  if(!strstr(binpath, name)){
    fprintf(stderr, "ERROR:test_binpath: lib_path not found correctly: %s with name %s\n", binpath, name);
    return 1;
  }

  printf("OK: lib_path: %s\n", binpath);
  return 0;
}

int main(int argc, char* argv[]){

  int i = test_exe_path();

  i += test_lib_path(argc, argv);

  return i;
}
