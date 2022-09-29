// used for C or C++ interfaces

#include "ffilesystem.h"

#ifdef __APPLE__
#include "TargetConditionals.h"
#endif

bool is_macos(){
#if TARGET_OS_MAC
  return true;
#else
  return false;
#endif
}

bool is_linux() {
#ifdef __linux__
  return true;
#else
  return false;
#endif
}

bool is_unix() {
#ifdef __unix__
  return true;
#else
  return false;
#endif
}

bool is_windows() {
#ifdef _WIN32
  return true;
#else
  return false;
#endif
}

bool sys_posix() {
  char sep[2];

  filesep(sep);
  return sep[0] == '/';
}


size_t get_maxp(){ return MAXP; }
