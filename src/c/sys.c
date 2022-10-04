#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#ifndef _MSC_VER
#include <unistd.h>
#endif

#include "ffilesystem.h"


// --- system calls for mkdir and copy_file
int fs_copy_file(const char* source, const char* destination, bool overwrite) {

if(source == NULL || strlen(source) == 0) {
  fprintf(stderr,"ERROR:filesystem:copy_file: source path %s must not be empty\n", source);
  return 1;
}
if(destination == NULL || strlen(destination) == 0) {
  fprintf(stderr, "ERROR:filesystem:copy_file: destination path %s must not be empty\n", destination);
  return 1;
}

  if(overwrite){
    if(fs_is_file(destination)){
      if(!fs_remove(destination)){
        fprintf(stderr, "ERROR:filesystem:copy_file: could not remove existing file %s\n", destination);
      }
    }
  }

  #ifdef _WIN32
  if(CopyFile(source, destination, true))
    return 0;
  return 1;
  #else
// from: https://wiki.sei.cmu.edu/confluence/pages/viewpage.action?pageId=87152177

  char* s = (char*) malloc(strlen(source) + 1);
  char* d = (char*) malloc(strlen(destination) + 1);
  strcpy(s, source);  // NOLINT(clang-analyzer-security.insecureAPI.strcpy)
  strcpy(d, destination);  // NOLINT(clang-analyzer-security.insecureAPI.strcpy)

  char *const args[4] = {"cp", s, d, NULL};

  int ret = execvp("cp", args);
  free(s);
  free(d);

  if(ret != -1)
    return 0;

  return ret;
  #endif
}

int fs_create_directories(const char* path) {
  // Windows: note that SHCreateDirectory is deprecated, so use a system call like Unix

  if(path == NULL || strlen(path) == 0) {
    fprintf(stderr,"ERROR:filesystem:mkdir: path %s must not be empty\n", path);
    return 1;
  }

  if(fs_is_dir(path))
    return 0;

  char* p = (char*) malloc(strlen(path) + 1);
  strcpy(p, path); // NOLINT(clang-analyzer-security.insecureAPI.strcpy)
#ifdef _WIN32
  fs_as_windows(p);
#endif

#ifdef _MSC_VER
  STARTUPINFO si = { 0 };
  PROCESS_INFORMATION pi;
  si.cb = sizeof(si);

  char* cmd = (char*) malloc(strlen(p) + 1 + 13);
  strcpy(cmd, "cmd /c mkdir ");
  strcat(cmd, p);
  free(p);

if(TRACE) printf("TRACE:mkdir %s\n", cmd);

  if (!CreateProcess(NULL, cmd, NULL, NULL, FALSE, 0, 0, 0, &si, &pi))
    return -1;

if(TRACE) printf("TRACE:mkdir waiting to complete %s\n", cmd);
  // Wait until child process exits.
  WaitForSingleObject( pi.hProcess, 2000 );
  CloseHandle(pi.hThread);
  CloseHandle(pi.hProcess);
  if(TRACE) printf("TRACE:mkdir completed %s\n", cmd);

  return 0;

#else
// from: https://wiki.sei.cmu.edu/confluence/pages/viewpage.action?pageId=87152177

#ifdef _WIN32
  char *const args[5] = {"cmd", "/c", "mkdir", p, NULL};
  int ret = execvp("cmd", args);
#else
  char *const args[4] = {"mkdir", "-p", p, NULL};
  int ret = execvp("mkdir", args);
#endif
  free(p);

  if(ret != -1)
    return 0;

  return ret;
#endif
}
