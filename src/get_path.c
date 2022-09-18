#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#ifndef FS_DLL_NAME
#define FS_DLL_NAME NULL
#warning "FS_DLL_NAME not defined, using NULL -- this will work like exe_path()"
#endif

#elif defined(HAVE_DLADDR)
#include <dlfcn.h>
static void dl_dummy_func() {}
#endif



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
