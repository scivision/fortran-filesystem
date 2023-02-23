#include <stdbool.h>

#include "ffilesystem.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

static bool fs_win32_is_symlink(const char* path)
{
#ifdef _WIN32
  DWORD a = GetFileAttributes(path);
  if(a == INVALID_FILE_ATTRIBUTES)
    return false;
  return a & FILE_ATTRIBUTE_REPARSE_POINT;
#else
  (void) path;
#endif

  return false;
}

static int fs_win32_create_symlink(const char* target, const char* link)
{
#ifdef _WIN32
  int p = SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE;

  if(fs_is_dir(target))
    p |= SYMBOLIC_LINK_FLAG_DIRECTORY;

  if (CreateSymbolicLink(link, target, p))
    return 0;

  DWORD err = GetLastError();
  fprintf(stderr, "ERROR:ffilesystem:CreateSymbolicLink: %ld\n", err);
  if(err == ERROR_PRIVILEGE_NOT_HELD){
    fprintf(stderr, "Enable Windows developer mode to use symbolic links:\n"
      "https://learn.microsoft.com/en-us/windows/apps/get-started/developer-mode-features-and-debugging\n");
  }
#else
  (void) target; (void) link;
#endif

  return -1;
}
