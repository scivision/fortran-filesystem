#include <string.h>

#ifdef _MSC_VER
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

size_t get_libpath(char* path)
{

#ifdef _MSC_VER
  char buf[MAX_PATH];
  if (GetModuleFileName(GetModuleHandle("mylib.dll"), buf, MAX_PATH) !=0)
  {
    strcpy(path, buf);
    return strlen(path);
  }
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
