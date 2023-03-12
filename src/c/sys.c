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

if(!fs_is_file(source)){
  fprintf(stderr, "ERROR:ffilesystem:copy_file: source file %s does not exist\n", source);
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
// so use a system call
//
// return 0 if successful, non-zero if not successful

  if(!path || strlen(path) == 0) {
    fprintf(stderr,"ERROR:ffilesystem:create_directories: path must not be empty\n");
    return 1;
  }

  if (fs_exists(path))
  {
    if(fs_is_dir(path))
      return 0;
    fprintf(stderr, "ERROR:filesystem:mkdir:create_directories: %s already exists but is not a directory\n", path);
    return 1;
  }

  char* p = (char*) malloc(MAXP);
  strncpy(p, path, MAXP-1);
  size_t L = strlen(path);
  p[L] = '\0';

  int r;
#ifdef _WIN32
  fs_as_windows(p);
  // don't directly specify "cmd.exe" in exec() for security reasons
  char* comspec = getenv("COMSPEC");
  if(!comspec){
    fprintf(stderr, "ERROR:ffilesystem:create_directories:exec: environment variable COMSPEC not defined\n");
    return 1;
  }
  intptr_t ir = _execl(comspec, "cmd", "/c", "mkdir", p, NULL);
  r = (int)ir;
#else
  r = execlp("mkdir", "mkdir", "-p", p, NULL);
#endif

  free(p);

  if(r != -1)
    return 0;

  return r;
}
