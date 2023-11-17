#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <unistd.h>

#include "ffilesystem.h"


// --- system calls for mkdir and copy_file
int fs_copy_file(const char* source, const char* destination, bool overwrite) {

if(!fs_is_file(source)) {
  fprintf(stderr,"ERROR:ffilesystem:copy_file: source file must exist\n");
  return 1;
}
if(strlen(destination) == 0) {
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

// from: https://wiki.sei.cmu.edu/confluence/pages/viewpage.action?pageId=87152177
  int r = execlp("cp", "cp", source, destination, NULL);
  return r != -1 ? 0 : r;
}

int fs_create_directories(const char* path) {
// Windows:
// * SHCreateDirectory is deprecated
// * CreateDirectory needs parents to exist
// so use a system call
//
// return 0 if successful, non-zero if not successful

  if(strlen(path) == 0) {
    fprintf(stderr, "ERROR:ffilesystem:create_directories: path must not be empty\n");
    return 1;
  }

  if(fs_exists(path)){
    if(fs_is_dir(path))
      return 0;

    fprintf(stderr, "ERROR:filesystem:create_directories: %s already exists but is not a directory\n", path);
    return 1;
  }

  char* p = (char*) malloc(FS_MAX_PATH);
  if(!p) return 1;
  strncpy(p, path, FS_MAX_PATH-1);
  size_t L = strlen(path);
  p[L] = '\0';

  int r;

  r = execlp("mkdir", "mkdir", "-p", p, NULL);

  free(p);

  return r != -1 ? 0 : r;
}
