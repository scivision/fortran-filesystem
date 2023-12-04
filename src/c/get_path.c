#ifdef __linux__
#define _GNU_SOURCE
#endif

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#if defined(HAVE_DLADDR)
#include <dlfcn.h>
static void dl_dummy_func() {}
#endif

#ifdef __APPLE__
#include <mach-o/dyld.h>
#elif defined(__linux__)
#include <unistd.h>
#endif

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

size_t fs_exe_path(char *path, size_t buffer_size)
{
  // https://stackoverflow.com/a/4031835
  // https://stackoverflow.com/a/1024937

#if defined(__linux__)
  // https://man7.org/linux/man-pages/man2/readlink.2.html
  size_t L = readlink("/proc/self/exe", path, buffer_size);
  if (L < 1)
    return 0;
  if (L >= buffer_size)
    L = buffer_size - 1;
  path[L] = '\0';
#elif defined(__APPLE__)
  char buf[buffer_size];
  uint32_t mp = sizeof(buf);
  if (_NSGetExecutablePath(buf, &mp) || !realpath(buf, path))
    return 0;
#else
  fprintf(stderr, "ERROR:ffilesystem:fs_exe_path: not implemented for this platform\n");
  return 0;
#endif

  return strlen(path);
}

size_t fs_lib_path(char *path, size_t buffer_size)
{

#if defined(HAVE_DLADDR)
  Dl_info info;

  if (!dladdr((void *)&dl_dummy_func, &info))
    return 0;

  size_t L = strlen(info.dli_fname);
  if(L > buffer_size){
    fprintf(stderr, "ERROR:ffilesystem:fs_lib_path: buffer_size %zu is too small\n", buffer_size);
    return 0;
  }

  strcpy(path, info.dli_fname);

  return L;
#endif

  (void)path;
  return 0 * buffer_size;
  // to avoid unused argument error
}
