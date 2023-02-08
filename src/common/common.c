#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ffilesystem.h"

#ifdef __APPLE__
#include "TargetConditionals.h"
#elif defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
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


void fs_as_posix(char* path)
{
// force posix file seperator
  if(!path)
    return;

  char s = '\\';
  char *p = strchr(path, s);
  while (p) {
    *p = '/';
    p = strchr(p+1, s);
  }
}

void fs_as_windows(char* path)
{
// as_windows() needed for system calls with MSVC
// force Windows file seperator
  if(!path)
    return;

  char s = '/';
  char *p = strchr(path, s);
  while (p) {
    *p = '\\';
    p = strchr(p+1, s);
  }
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
  strncpy(result, buf2, buffer_size);  // NOLINT(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
  result[L1] = '\0';
  free(buf);
  free(buf2);
  return L1;
}


size_t fs_exe_dir(char* path, size_t buffer_size)
{
  char* buf = (char*) malloc(buffer_size);

  fs_exe_path(buf, buffer_size);

  size_t L = fs_parent(buf, path, buffer_size);

  free(buf);
  return L;

}

size_t fs_lib_dir(char* path, size_t buffer_size)
{
  char* buf = (char*) malloc(buffer_size);

  fs_lib_path(buf, buffer_size);

  size_t L = fs_parent(buf, path, buffer_size);

  free(buf);
  return L;

}

bool _fs_win32_is_symlink(const char* path)
{
  if (!path || strlen(path) == 0)
    return false;
  // need zero length check on Windows else could return incorrect true.

#ifdef _WIN32
  return GetFileAttributes(path) & FILE_ATTRIBUTE_REPARSE_POINT;
#endif
  return false;
}

int _fs_win32_create_symlink(const char* target, const char* link)
{
#ifdef _WIN32
  int p = SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE;

  if(fs_is_dir(target))
    p |= SYMBOLIC_LINK_FLAG_DIRECTORY;

  bool s = CreateSymbolicLink(link, target, p);

  if (s)
    return 0;

  DWORD err = GetLastError();
  fprintf(stderr, "ERROR:ffilesystem:CreateSymbolicLink: %ld\n", err);
  if(err == ERROR_PRIVILEGE_NOT_HELD){
    fprintf(stderr, "Enable Windows developer mode to use symbolic links:\n"
      "https://learn.microsoft.com/en-us/windows/apps/get-started/developer-mode-features-and-debugging\n");
  }
  return -1;
#else
  (void) target; (void) link;
  return 1;
#endif
}
