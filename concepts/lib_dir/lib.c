#ifdef __linux__
#define _GNU_SOURCE
#endif

#include <string.h>

#ifdef _WIN32
#ifndef MY_DLL_NAME
#define MY_DLL_NAME NULL
#endif
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#include <dlfcn.h>
#endif


size_t get_libpath(char* path)
{

#ifdef _WIN32
  if (GetModuleFileName(GetModuleHandle(MY_DLL_NAME), path, MAX_PATH) !=0)
    return strlen(path);
#else
 Dl_info info;

 if (dladdr(get_libpath, &info))
 {
   strcpy(path, info.dli_fname);
   return strlen(path);
 }
#endif

  return 0;
  // always have a return

}
