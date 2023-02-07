#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <process.h>
#else
#include <unistd.h>
#endif

#include "ffilesystem.h"


// --- system calls for mkdir and copy_file
int fs_copy_file(const char* source, const char* destination, bool overwrite) {

if(source == NULL || strlen(source) == 0) {
  fprintf(stderr,"ERROR:ffilesystem:copy_file: source path must not be empty\n");
  return 1;
}
if(destination == NULL || strlen(destination) == 0) {
  fprintf(stderr, "ERROR:ffilesystem:copy_file: destination path must not be empty\n");
  return 1;
}

  if(overwrite){
    if(fs_is_file(destination)){
      if(!fs_remove(destination)){
        fprintf(stderr, "ERROR:ffilesystem:copy_file: could not remove existing file %s\n", destination);
      }
    }
  }

#ifdef _WIN32
  if(CopyFile(source, destination, true))
    return 0;
  return 1;
#else
// from: https://wiki.sei.cmu.edu/confluence/pages/viewpage.action?pageId=87152177

  int ret = execlp("cp", "cp", source, destination, NULL);

  if(ret != -1)
    return 0;

  return ret;
#endif
}

int fs_create_directories(const char* path) {
// Windows: SHCreateDirectory is deprecated, CreateDirectory needs parents to exist,
// so use a system call like Unix

  if(!path || strlen(path) == 0) {
    fprintf(stderr,"ERROR:ffilesystem:mkdir: path must not be empty\n");
    return 1;
  }

  if(fs_is_dir(path))
    return 0;

  char* p = (char*) malloc(strlen(path) + 1);
  strcpy(p, path); // NOLINT(clang-analyzer-security.insecureAPI.strcpy)
#ifdef _WIN32
  fs_as_windows(p);
#endif

int r;
#ifdef _WIN32
  intptr_t ir = _execlp("cmd", "cmd", "/c", "mkdir", p, NULL);
  r = (int)ir;
#else
  r = execlp("mkdir", "mkdir", "-p", p, NULL);
#endif

  free(p);

  if(r != -1)
    return 0;

  return r;
}
