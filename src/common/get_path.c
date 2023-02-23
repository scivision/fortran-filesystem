#ifdef __linux__
#define _GNU_SOURCE
#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#elif defined(HAVE_DLADDR)
#include <dlfcn.h>
static void dl_dummy_func() {}
#endif

#ifdef __APPLE__
#include <mach-o/dyld.h>
#elif defined(__OpenBSD__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__DragonFly__)
#include <sys/sysctl.h>
#elif defined(__linux__)
#include <unistd.h>
#endif


size_t fs_exe_path(char* path, size_t buffer_size)
{
  // https://stackoverflow.com/a/4031835
  // https://stackoverflow.com/a/1024937

  if(buffer_size == 0){
    path = NULL;
    return 0;
  }

#ifdef _WIN32
 // https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulefilenamea
  DWORD r = GetModuleFileName(NULL, path, (DWORD)buffer_size);
  if (r == 0){
    path = NULL;
    return 0;
  }
#elif defined(__linux__)
  // https://man7.org/linux/man-pages/man2/readlink.2.html
  size_t L = readlink("/proc/self/exe", path, buffer_size);
  if (L < 1 || L >= buffer_size){
    path = NULL;
    return 0;
  }
  path[L] = '\0';
#elif defined(__APPLE__)
  char buf[buffer_size];
  uint32_t mp = sizeof(buf);
  if (_NSGetExecutablePath(buf, &mp) != 0){
    path = NULL;
    return 0;
  }
  if(!realpath(buf, path))
    return 0;
#elif defined(__OpenBSD__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__DragonFly__)
  char* buf = (char*) malloc(buffer_size);
  int mib[4];
  mib[0] = CTL_KERN;
  mib[1] = KERN_PROC;
  mib[2] = KERN_PROC_PATHNAME;
  mib[3] = -1;
  size_t cb = sizeof(buf);

  if(sysctl(mib, 4, buf, &cb, NULL, 0) != 0){
    path = NULL;
    free(buf);
    return 0;
  }
  if(!realpath(buf, path)){
    path = NULL;
    free(buf);
    return 0;
  }
  free(buf);
#else
  path = NULL;
  return 0;
#endif

  return strlen(path);
}


size_t fs_lib_path(char* path, size_t buffer_size)
{
  if(buffer_size == 0){
    path = NULL;
    return 0;
  }

#if defined(_WIN32) && defined(FS_DLL_NAME)
 // https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulefilenamea
  DWORD r = GetModuleFileName(GetModuleHandle(FS_DLL_NAME), path, (DWORD)buffer_size);
  if (r == 0){
    path = NULL;
    return 0;
  }
#elif defined(HAVE_DLADDR)
  Dl_info info;

  if (dladdr( (void*)&dl_dummy_func, &info) == 0){
    path = NULL;
    return 0;
  }

  size_t L = strlen(info.dli_fname);

  if(L >= buffer_size){
    path = NULL;
    fprintf(stderr, "ERROR:filesystem:fs_lib_path: buffer too small\n");
    return 0;
  }

  strncpy(path, info.dli_fname, buffer_size); // NOLINT(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
  path[L] = '\0';
#else
  path = NULL;
  return 0*buffer_size;
  // to avoid unused argument error
#endif

  return strlen(path);
}
