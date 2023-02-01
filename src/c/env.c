#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#ifdef _MSC_VER
#include <direct.h>
#else
#include <unistd.h>
#endif

#include "ffilesystem.h"


size_t _fs_getenv(const char* name, char* result, size_t buffer_size);


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

#ifdef _WIN32
  return _fs_getenv("USERPROFILE", result, buffer_size);
#else
  return _fs_getenv("HOME", result, buffer_size);
#endif
}

size_t fs_get_tempdir(char* result, size_t buffer_size) {

#ifdef _WIN32
  return _fs_getenv("TEMP", result, buffer_size);
#else
  return _fs_getenv("TMPDIR", result, buffer_size);
#endif

}


size_t _fs_getenv(const char* name, char* result, size_t buffer_size) {

char* buf;

#ifdef _MSC_VER
  buf = (char*) malloc(buffer_size);
  if(getenv_s(&L, buf, buffer_size, name) != 0){
    fprintf(stderr, "ERROR:ffilesystem:getenv: %s\n", strerror(errno));
    free(buf);
    result = NULL;
    return 0;
  }
#else
  buf = getenv(name);
#endif

  size_t L = fs_normal(buf, result, buffer_size);

#ifdef _MSC_VER
  free(buf);
#endif

return L;

}
