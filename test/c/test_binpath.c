#include "ffilesystem.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int test_exe_path(void){

  char binpath[MAXP];

  exe_path(binpath);
  if (!strstr(binpath, "test_binpath")) {
    fprintf(stderr, "ERROR:test_binpath: exe_path not found correctly: %s\n", binpath);
    return 1;
  }

  printf("OK: exe_path: %s\n", binpath);
  return 0;
}

int test_lib_path(char* argv[]){

  char binpath[MAXP];

  if(argv[1] == NULL){
    fprintf(stderr, "need argument 0 for static or 1 for shared.  Got: %s\n", argv[1]);
    return 1;
  }

  int shared = atoi(argv[1]);

  if(!shared) {
    printf("SKIPPED: lib_path: static library\n");
    return 0;
  }

  lib_path(binpath);

  char name[18];
  if (is_macos()) {
    strcpy(name, "ffilesystem.dylib");
  }
  else if(is_windows()) {
    strcpy(name, "ffilesystem.dll");
  }
  else{
    strcpy(name, "libffilesystem.so");
  }

  if(!strstr(binpath, name)){
    fprintf(stderr, "ERROR:test_binpath: lib_path not found correctly: %s with name %s\n", binpath, name);
    return 1;
  }

  printf("OK: lib_path: %s\n", binpath);
  return 0;
}

int main(int argc, char* argv[]){

  int i = test_exe_path();

  i += test_lib_path(argv);

  return i;
}
