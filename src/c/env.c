#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
#include <direct.h>
#else
#include <unistd.h>
#endif

#include "ffilesystem.h"


static size_t fs_getenv(const char* name, char* path, size_t buffer_size)
{
  // <stdlib.h>
  char* buf = getenv(name);
  if(!buf) // not error because sometimes we just check if envvar is defined
    goto retnull;

  if(strlen(buf) >= buffer_size){
    fprintf(stderr, "ERROR:ffilesystem:getenv: buffer too small\n");
    goto retnull;
  }

  return fs_normal(buf, path, buffer_size);

retnull:
  path = NULL;
  return 0;
}


size_t fs_get_cwd(char* path, size_t buffer_size)
{
// https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/getcwd-wgetcwd?view=msvc-170
  // <direct.h> / <unistd.h>
  char* x = getcwd(path, buffer_size);

  if(!x || strlen(x) >= buffer_size){
    fprintf(stderr, "ERROR:ffilesystem:getcwd\n");
    goto nullret;
  }

  return fs_normal(path, path, buffer_size);

nullret:
  path = NULL;
  return 0;
}

size_t fs_get_homedir(char* path, size_t buffer_size)
{
  if(fs_is_windows()){
    return fs_getenv("USERPROFILE", path, buffer_size);
  }
  else{
    return fs_getenv("HOME", path, buffer_size);
  }
}

size_t fs_get_tempdir(char* path, size_t buffer_size)
{
  size_t L;
  if(fs_is_windows()){
    L = fs_getenv("TEMP", path, buffer_size);
  }
  else{
    L = fs_getenv("TMPDIR", path, buffer_size);
  }
  if(L > 0)
    return L;

  if (fs_is_dir("/tmp") && buffer_size > 4){
    strcpy(path, "/tmp");
    return 4;
  }

  fprintf(stderr, "ERROR:ffilesystem:get_tempdir: could not find temp dir\n");
  path = NULL;
  return 0;
}
