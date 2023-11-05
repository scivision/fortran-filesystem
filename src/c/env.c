#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <pwd.h>
#include <errno.h>

#include <unistd.h>

#include "ffilesystem.h"


static size_t fs_getenv(const char* name, char* path, size_t buffer_size)
{
  // <stdlib.h>
  char* buf = getenv(name);
  if(!buf) // not error because sometimes we just check if envvar is defined
    return 0;

  if(strlen(buf) >= buffer_size)
    buf[buffer_size-1] = '\0';

  return fs_normal(buf, path, buffer_size);
}


size_t fs_get_cwd(char* path, size_t buffer_size)
{
// https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/getcwd-wgetcwd?view=msvc-170
  // <direct.h> / <unistd.h>
  char* x = getcwd(path, buffer_size);

  if(!x)
    return 0;

  if(strlen(x) >= buffer_size)
    path[buffer_size-1] = '\0';

  return fs_normal(path, path, buffer_size);
}

size_t fs_get_homedir(char* path, size_t buffer_size)
{
  size_t L = fs_getenv("HOME", path, buffer_size);
  if (L)
    return L;

  const char *h = getpwuid(geteuid())->pw_dir;
  if (!h)
    return 0;

  return fs_normal(h, path, buffer_size);

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
