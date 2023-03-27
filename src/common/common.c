#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "ffilesystem.h"

#ifdef __APPLE__
#include "TargetConditionals.h"
#endif

bool fs_is_macos(){
#if TARGET_OS_MAC
  return true;
#else
  return false;
#endif
}

bool fs_is_linux() {
#ifdef __linux__
  return true;
#else
  return false;
#endif
}

bool fs_is_unix() {
#ifdef __unix__
  return true;
#else
  return fs_is_macos();
#endif
}

bool fs_is_windows() {
#ifdef _WIN32
  return true;
#else
  return false;
#endif
}

bool fs_is_cygwin(){
#ifdef __CYGWIN__
  return true;
#else
  return false;
#endif
}


size_t fs_get_maxp(){ return MAXP; }


size_t fs_compiler(char* name, size_t buffer_size)
{
  if(!name || buffer_size == 0){
    name = NULL;
    return 0;
  }

int L=0;

#if defined(__INTEL_LLVM_COMPILER)
  L = snprintf(name, buffer_size, "Intel LLVM %d %s", __INTEL_LLVM_COMPILER,  __VERSION__);
#elif defined(__NVCOMPILER_LLVM__)
  L = snprintf(name, buffer_size, "NVIDIA nvc %d.%d.%d", __NVCOMPILER_MAJOR__, __NVCOMPILER_MINOR__, __NVCOMPILER_PATCHLEVEL__);
#elif defined(__clang__)
  L = snprintf(name, buffer_size, "Clang %d.%d.%d", __clang_major__, __clang_minor__, __clang_patchlevel__);
#elif defined(__GNUC__)
  L = snprintf(name, buffer_size, "GNU GCC %d.%d.%d", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#elif defined(_MSC_VER)
  L = snprintf(name, buffer_size, "MSVC %d", _MSC_FULL_VER);
#else
  name[0] = '\0';
#endif

if (L < 0){
  fprintf(stderr, "ERROR:ffilesystem:fs_compiler: snprintf failed\n");
  L = 0;
}

  return L;
}


size_t fs_make_absolute(const char* path, const char* top_path,
                        char* result, size_t buffer_size)
{
  size_t L1 = fs_expanduser(path, result, buffer_size);

  if (L1 > 0 && fs_is_absolute(result))
    return L1;

  char* buf = (char*) malloc(buffer_size);
  size_t L2 = fs_expanduser(top_path, buf, buffer_size);
  if(L2 == 0){
    free(buf);
    return L1;
  }

  char* buf2 = (char*) malloc(buffer_size);
  L1 = fs_join(buf, result, buf2, buffer_size);
  strncpy(result, buf2, buffer_size);
  result[L1] = '\0';
  free(buf);
  free(buf2);
  return L1;
}
