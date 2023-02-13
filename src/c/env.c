#define __STDC_WANT_LIB_EXT1__ 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


size_t _fs_getenv(const char* name, char* path, size_t buffer_size)
{
  if(buffer_size == 0){
    path = NULL;
    return 0;
  }

  char* buf;
  size_t L;

#ifdef HAVE_GETENV_S
  buf = (char*) malloc(buffer_size);
  if(getenv_s(&L, buf, buffer_size, name) != 0){
    fprintf(stderr, "ERROR:ffilesystem:getenv: %s\n", strerror(errno));
    free(buf);
    path = NULL;
    return 0;
  }
#else
  // <stdlib.h>
  buf = getenv(name);
  if(!buf){
    path = NULL;
    return 0;
  }
  if(strlen(buf) >= buffer_size){
    fprintf(stderr, "ERROR:ffilesystem:getenv: buffer too small\n");
    path = NULL;
    return 0;
  }
#endif

  L = fs_normal(buf, path, buffer_size);

#ifdef HAVE_GETENV_S
  free(buf);
#endif

  return L;
}


size_t fs_get_cwd(char* path, size_t buffer_size)
{
  if(buffer_size == 0){
    path = NULL;
    return 0;
  }

  char* x;

#ifdef _MSC_VER
// https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/getcwd-wgetcwd?view=msvc-170
  x = _getcwd(path, (int)buffer_size);
#else
  x = getcwd(path, buffer_size);
#endif

  if(!x || strlen(x) >= buffer_size){
    fprintf(stderr, "ERROR:ffilesystem:getcwd\n");
    path = NULL;
    return 0;
  }

  return fs_normal(path, path, buffer_size);

}

size_t fs_get_homedir(char* path, size_t buffer_size)
{
#ifdef _WIN32
  char name[] = "USERPROFILE";
#else
  char name[] = "HOME";
#endif

  return _fs_getenv(name, path, buffer_size);

}

size_t fs_get_tempdir(char* path, size_t buffer_size)
{
#ifdef _WIN32
  char name[] = "TEMP";
#else
  char name[] = "TMPDIR";
#endif

  size_t L = _fs_getenv(name, path, buffer_size);
  if(L > 0){
    return L;
  }
  else if (fs_is_dir("/tmp") && buffer_size > 4){
    strncpy(path, "/tmp", buffer_size);
    path[4] = '\0';
    return 4;
  }
  else{
    fprintf(stderr, "ERROR:ffilesystem:get_tempdir: could not find temp dir\n");
    path = NULL;
    return 0;
  }

}
