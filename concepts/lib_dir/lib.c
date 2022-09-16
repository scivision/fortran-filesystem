#include <string.h>
#include <dlfcn.h>

size_t get_libpath(char* path)
{
 Dl_info info;

 if (dladdr(get_libpath, &info))
 {
    strcpy(path, info.dli_fname);
 }
else
  {
      strcpy(path, NULL);
  }

 return strlen(path);
}
