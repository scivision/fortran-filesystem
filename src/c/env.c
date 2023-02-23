#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
#include <direct.h>
#else
#include <unistd.h>
#endif

#include "ffilesystem.h"


size_t _fs_getenv(const char* name, char* path, size_t buffer_size)
{
  if(buffer_size == 0) goto retnull;

  char* buf;

  // <stdlib.h>
  buf = getenv(name);
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
  if(buffer_size == 0) goto nullret;

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
