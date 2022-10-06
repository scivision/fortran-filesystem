#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ffilesystem.h"


int test_exe_path(char* argv[]){

  char binpath[MAXP], bindir[MAXP], p[MAXP];

  fs_exe_path(binpath, MAXP);
  if (!strstr(binpath, argv[2])) {
    fprintf(stderr, "ERROR:test_binpath: exe_path not found correctly: %s\n", binpath);
    return 1;
  }

  size_t L = fs_exe_dir(bindir, MAXP);
  if(L == 0){
    fprintf(stderr, "ERROR:test_binpath: exe_dir not found correctly: %s\n", bindir);
    return 1;
  }
  fs_parent(binpath, p, MAXP);

  if(!fs_equivalent(bindir, p)){
    fprintf(stderr, "ERROR:test_binpath: exe_dir and parent(exe_path) should be equivalent: %s %s\n", bindir, p);
    return 1;
  }

  printf("OK: exe_path: %s\n", binpath);
  printf("OK: exe_dir: %s\n", bindir);
  return 0;
}

int test_lib_path(int argc, char* argv[]){

  char binpath[MAXP], bindir[MAXP], p[MAXP];

  if(argc < 2){
    fprintf(stderr, "need argument 0 for static or 1 for shared.  Got: %s\n", argv[1]);
    return 1;
  }

  int shared = atoi(argv[1]);

  size_t L = fs_lib_path(binpath, MAXP);
  size_t L2 = fs_lib_dir(bindir, MAXP);

  if(!shared) {
    if (L != 0 || L2 != 0) {
      fprintf(stderr, "ERROR:test_binpath: lib_path and lib_dir should be empty length 0: %s %ju\n", binpath, L);
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

  fs_parent(binpath, p, MAXP);

  if(!fs_equivalent(bindir, p)){
    fprintf(stderr, "ERROR:test_binpath: lib_dir and parent(lib_path) should be equivalent: %s %s\n", bindir, p);
    return 1;
  }

  printf("OK: lib_path: %s\n", binpath);
  printf("OK: lib_dir: %s\n", bindir);
  return 0;
}

int main(int argc, char* argv[]){

  int i = test_exe_path(argv);

  i += test_lib_path(argc, argv);

  return i;
}
