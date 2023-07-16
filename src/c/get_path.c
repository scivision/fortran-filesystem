#define _GNU_SOURCE

#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#if defined(HAVE_DLADDR)
#include <dlfcn.h>
static void dl_dummy_func() {}
#endif

#include <unistd.h>

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

size_t fs_exe_path(char *path, size_t buffer_size)
{
  // https://stackoverflow.com/a/4031835
  // https://stackoverflow.com/a/1024937
  // https://man7.org/linux/man-pages/man2/readlink.2.html
  size_t L = readlink("/proc/self/exe", path, buffer_size);
  if (L < 1)
    return 0;
  if (L >= buffer_size)
    L = buffer_size - 1;
  path[L] = '\0';

  return strlen(path);
}

size_t fs_lib_path(char *path, size_t buffer_size)
{

#if defined(HAVE_DLADDR)
  Dl_info info;

  if (!dladdr((void *)&dl_dummy_func, &info))
    return 0;

  size_t L = strlen(info.dli_fname);
  size_t N = min(L, buffer_size - 1);

  strncpy(path, info.dli_fname, N);
  path[N + 1] = '\0';
  return N;
#endif

  (void)path;
  return 0 * buffer_size;
  // to avoid unused argument error
}
