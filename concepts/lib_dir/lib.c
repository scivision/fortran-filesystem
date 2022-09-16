#include <string.h>

#ifdef _MSC_VER
#ifndef MY_DLL_NAME
#error "must define MY_DLL_PATH with filename of this file's DLL"
#endif
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#include <dlfcn.h>
#endif


size_t get_libpath(char* path)
{

#ifdef _MSC_VER
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
}
