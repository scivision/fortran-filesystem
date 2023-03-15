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

int test_lib_path(char* argv[]){

  char binpath[MAXP], bindir[MAXP], p[MAXP];

  int shared = atoi(argv[1]);

  size_t L = fs_lib_path(binpath, MAXP);
  size_t L2 = fs_lib_dir(bindir, MAXP);

  if(!shared) {
    if (L != 0 || L2 != 0) {
      fprintf(stderr, "ERROR:test_binpath: lib_path and lib_dir should be empty length 0: %s %zu\n", binpath, L);
      return 1;
    }
    fprintf(stderr, "SKIP: lib_path: feature not available\n");
    return 0;
  }


  if(!strstr(binpath, argv[3])){
    fprintf(stderr, "ERROR:test_binpath: lib_path not found correctly: %s does not contain %s\n", binpath, argv[3]);
    return 1;
  }

  fs_parent(binpath, p, MAXP);

  if(!fs_equivalent(bindir, p)){
    fprintf(stderr, "ERROR:test_binpath_c: lib_dir and parent(lib_path) should be equivalent: %s %s\n", bindir, p);
    return 1;
  }

  printf("OK: lib_path: %s\n", binpath);
  printf("OK: lib_dir: %s\n", bindir);
  return 0;
}

int main(int argc, char* argv[]){

  if (argc < 4) {
    fprintf(stderr, "ERROR: test_binpath_c: not enough arguments\n");
    return 1;
  }

  int i = test_exe_path(argv);

  i += test_lib_path(argv);

  return i;
}
