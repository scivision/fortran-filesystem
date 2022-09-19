#include "filesystem.h"

bool is_macos(){
#if __APPLE__
#include "TargetConditionals.h"
#if TARGET_OS_MAC
  return true;
#endif
#endif
return false;
}

bool is_linux() {
#ifdef __linux__
  return true;
#endif
return false;
}

bool is_unix() {
#ifdef __unix__
  return true;
#endif
return false;
}

bool is_windows() {
#ifdef _WIN32
  return true;
#endif
return false;
}

bool sys_posix() {
  char sep[2];

  filesep(sep);
  return sep[0] == '/';
}


size_t get_maxp(){ return MAXP; }
