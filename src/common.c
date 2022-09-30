// used for C or C++ interfaces

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
  return false;
#endif
}

bool fs_is_windows() {
#ifdef _WIN32
  return true;
#else
  return false;
#endif
}


size_t fs_get_maxp(){ return MAXP; }
