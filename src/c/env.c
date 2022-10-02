#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#ifndef _MSC_VER
#include <unistd.h>
#endif

#include "ffilesystem.h"


size_t fs_get_cwd(char* result, size_t buffer_size) {

#ifdef _MSC_VER
// https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/getcwd-wgetcwd?view=msvc-170
  if (_getcwd(result, (DWORD)buffer_size) == NULL){
    result[0] = '\0';
    return 0;
  }
#else
  if (getcwd(result, buffer_size) == NULL){
    result[0] = '\0';
    return 0;
  }
#endif

  return fs_normal(result, result, buffer_size);
}

size_t fs_get_homedir(char* result, size_t buffer_size) {

char* buf;
size_t L;

#ifdef _WIN32
  buf = (char*) malloc(buffer_size);
  if(getenv_s(&L, buf, buffer_size, "USERPROFILE") != 0){
    fprintf(stderr, "ERROR:get_homedir: %s\n", strerror(errno));
    free(buf);
    result = NULL;
    return 0;
  }
#else
  buf = getenv("HOME");
#endif
  L = fs_normal(buf, result, buffer_size);
  if(TRACE) printf("TRACE: get_homedir: %s %s\n", buf, result);
#ifdef _WIN32
  free(buf);
#endif
  return L;
}

size_t fs_get_tempdir(char* result, size_t buffer_size) {

char* buf;
size_t L;

#ifdef _WIN32
  buf = (char*) malloc(buffer_size);
  if(GetTempPath((DWORD)buffer_size, buf) == 0){
    fprintf(stderr, "ERROR:get_tempdir: %s\n", strerror(errno));
    free(buf);
    result = NULL;
    return 0;
  }
#else
  buf = getenv("TMPDIR");
#endif

  L = fs_normal(buf, result, buffer_size);
#ifdef _WIN32
  free(buf);
#endif
  return L;
}
