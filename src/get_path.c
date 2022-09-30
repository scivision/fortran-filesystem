#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#ifndef FS_DLL_NAME
#define FS_DLL_NAME NULL
#warning "FS_DLL_NAME not defined, using NULL -- this will work like fs_exe_path()"
#endif

#else
#include <unistd.h>

#ifdef HAVE_DLADDR
#include <dlfcn.h>
static void dl_dummy_func() {}
#endif
#endif

#ifdef __APPLE__
#include <mach-o/dyld.h>
#elif defined(__OpenBSD__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__DragonFly__)
#include <sys/sysctl.h>
#endif



size_t fs_exe_path(char* path, size_t buffer_size){
  // https://stackoverflow.com/a/4031835
  // https://stackoverflow.com/a/1024937

#ifdef _WIN32
 // https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulefilenamea
  return GetModuleFileName(NULL, path, (DWORD)buffer_size);
#elif defined(__linux__)
  // https://man7.org/linux/man-pages/man2/readlink.2.html
  if (readlink("/proc/self/exe", path, buffer_size) == -1)
    return 0;
#elif defined(__APPLE__)
  char buf[buffer_size];
  uint32_t mp = sizeof(buf);
  if (_NSGetExecutablePath(buf, &mp) != 0)
    return 0;
  if(realpath(buf, path) == NULL)
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


size_t fs_lib_path(char* path, size_t buffer_size){

#ifdef _WIN32
 // https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulefilenamea
  return GetModuleFileName(GetModuleHandle(FS_DLL_NAME), path, (DWORD)buffer_size);
#elif defined(HAVE_DLADDR)
  Dl_info info;

  if (dladdr( (void*)&dl_dummy_func, &info) != 0)
  {
    strncpy(path, info.dli_fname, buffer_size);
    path[strlen(path)] = '\0';
    return strlen(path);
  }
#endif

  (void)buffer_size;
  (void)path;
  return 0;

}
