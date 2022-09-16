#include <string.h>

#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

size_t get_libpath(char* path)
{

#ifdef _MSC_VER
  if (GetModuleFileName(GetModuleHandle("mylib.dll"), path, MAX_PATH) !=0)
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
