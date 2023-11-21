#include <stdbool.h>
#include <string.h>

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

bool fs_is_mingw(){
#ifdef __MINGW32__
  return true;
#else
  return false;
#endif
}
