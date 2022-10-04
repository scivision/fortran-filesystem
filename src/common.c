#include <stdlib.h>
#include <string.h>

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


void fs_as_posix(char* path) {
// force posix file seperator
  char s = '\\';
  char *p = strchr(path, s);
  while (p) {
    *p = '/';
    p = strchr(p+1, s);
  }
}

void fs_as_windows(char* path) {
// as_windows() needed for system calls with MSVC
// force Windows file seperator
  char s = '/';
  char *p = strchr(path, s);
  while (p) {
    *p = '\\';
    p = strchr(p+1, s);
  }
}


size_t fs_make_absolute(const char* path, const char* top_path, char* result, size_t buffer_size){

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
  strncpy(result, buf2, buffer_size);  // NOLINT(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
  result[L1] = '\0';
  free(buf);
  free(buf2);
  return L1;
}
