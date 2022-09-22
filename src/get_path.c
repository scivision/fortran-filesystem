#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define MAXP _MAX_PATH

#ifndef FS_DLL_NAME
#define FS_DLL_NAME NULL
#warning "FS_DLL_NAME not defined, using NULL -- this will work like exe_path()"
#endif

#else
#include <unistd.h>

#ifdef HAVE_DLADDR
#include <dlfcn.h>
static void dl_dummy_func() {}
#endif
#endif

#ifdef __APPLE__
#include <sys/syslimits.h>
#include <mach-o/dyld.h>
#define MAXP PATH_MAX
#elif defined(__OpenBSD__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__DragonFly__)
#include <sys/sysctl.h>
#elif defined(__linux__)
#include <limits.h>
#ifdef PATH_MAX
#define MAXP PATH_MAX
#endif
#endif

#ifndef MAXP
#define MAXP 256
#endif


size_t exe_path(char* path){
  // https://stackoverflow.com/a/4031835
  // https://stackoverflow.com/a/1024937

#ifdef _WIN32
 // https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulefilenamea
  return GetModuleFileName(NULL, path, MAXP);
#elif defined(__linux__)
  // https://man7.org/linux/man-pages/man2/readlink.2.html
  if (readlink("/proc/self/exe", path, MAXP) == -1)
    return 0;
#elif defined(__APPLE__)
  char buf[MAXP];
  uint32_t mp = sizeof(buf);
  if (_NSGetExecutablePath(buf, &mp) != 0)
    return 0;
  if(realpath(buf, path) == NULL)
    return 0;
#elif defined(__OpenBSD__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__DragonFly__)
  char* buf = (char*) malloc(MAXP);
  int mib[4];
  mib[0] = CTL_KERN;
  mib[1] = KERN_PROC;
  mib[2] = KERN_PROC_PATHNAME;
  mib[3] = -1;
  size_t cb = sizeof(buf);

  if(sysctl(mib, 4, buf, &cb, NULL, 0) != 0){
    free(buf);
    return 0;
  }
  if(realpath(buf, path) == NULL){
    free(buf);
    return 0;
  }
  free(buf);
#else
  return 0;
#endif

  return strlen(path);

}


size_t lib_path(char* path){

#ifdef _WIN32
 // https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulefilenamea
  return GetModuleFileName(GetModuleHandle(FS_DLL_NAME), path, _MAX_PATH);
#elif defined(HAVE_DLADDR)
  Dl_info info;

  if (dladdr( (void*)&dl_dummy_func, &info) != 0)
  {
    strcpy(path, info.dli_fname);
    return strlen(path);
  }
#endif

  return 0;

}
