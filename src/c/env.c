#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <sys/types.h>
#include <errno.h>

#ifdef _WIN32
#include <direct.h>
#else
#include <pwd.h>
#include <unistd.h>
#endif

#include "ffilesystem.h"


static size_t fs_getenv(const char* name, char* path, size_t buffer_size)
{
  // <stdlib.h>
  char* buf = getenv(name);
  if(!buf) // not error because sometimes we just check if envvar is defined
    return 0;
  else if(strlen(buf) >= buffer_size){
    fprintf(stderr, "ERROR:ffilesystem:fs_getenv: buffer_size %zu is too small for %s\n", buffer_size, name);
    return 0;
  }

  return fs_normal(buf, path, buffer_size);
}


size_t fs_get_cwd(char* path, size_t buffer_size)
{
// <direct.h> https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/getcwd-wgetcwd?view=msvc-170
// <unistd.h> https://www.man7.org/linux/man-pages/man3/getcwd.3.html
#ifdef _WIN32
  char* x = _getcwd(path, buffer_size);
#else
  char* x = getcwd(path, buffer_size);
#endif

  if(!x) {
    fprintf(stderr, "ERROR:ffilesystem:fs_get_cwd: %s\n", strerror(errno));
    return 0;
  }

  if(FS_TRACE) printf("TRACE:fs_get_cwd: %s  %s   buffer_size %zu  strlen %zu\n", x, path, buffer_size, strlen(path));

  return fs_normal(path, path, buffer_size);
}

size_t fs_get_homedir(char* path, size_t buffer_size)
{
  size_t L = fs_getenv(fs_is_windows() ?  "USERPROFILE" : "HOME", path, buffer_size);
  if (L)
    return L;

#ifdef _WIN32
  return 0;
#else
  const char *h = getpwuid(geteuid())->pw_dir;
  if (!h)
    return 0;

  return fs_normal(h, path, buffer_size);
#endif
}

size_t fs_get_tempdir(char* path, size_t buffer_size)
{
  size_t L = fs_getenv("TMPDIR", path, buffer_size);
  if(L)
    return L;

  if (buffer_size > 4 && fs_is_dir("/tmp")){
    strcpy(path, "/tmp");
    return 4;
  }

  return 0;
}
