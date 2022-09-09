// https://man7.org/linux/man-pages/man3/realpath.3.html
// https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man3/realpath.3.html
// https://linux.die.net/man/3/realpath
// https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fullpath-wfullpath?view=msvc-170

#include <stdlib.h>
#include <string.h>


size_t fs_realpath(const char* path, char* r) {
  if (path == NULL || strlen(path) == 0) {
    r = NULL;
    return 0;
  }

#ifdef _WIN32
  _fullpath(r, path, _MAX_PATH);
#else
  realpath(path, r);
#endif

  return strlen(r);
}
