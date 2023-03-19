#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>


size_t fs_win32_read_symlink(const char* path, char* r, size_t buffer_size)
{
  // this resolves Windows symbolic links (reparse points and junctions)
  // it also resolves the case insensitivity of Windows paths to the disk case
  // References:
  // https://stackoverflow.com/a/50182947
  // https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getfinalpathnamebyhandlea

  HANDLE h = CreateFileA(path, 0, 0, NULL, OPEN_EXISTING,
    FILE_FLAG_BACKUP_SEMANTICS, NULL);
  if(h == INVALID_HANDLE_VALUE){
    fprintf(stderr, "ERROR:win32_read_symlink: %s failed open CreateFile\n", path);
    return 0;
  }
  DWORD L = GetFinalPathNameByHandleA(h, r, buffer_size, FILE_NAME_NORMALIZED);
  CloseHandle(h);
  if (L == 0){
    fprintf(stderr, "ERROR:win32_read_symlink: %s failed GetFinalPathNameByHandle\n", path);
    return 0;
  }
  else if (L >= MAX_PATH){
    fprintf(stderr, "ERROR:win32_read_symlink: %s failed GetFinalPathNameByHandle: buffer too small\n", path);
    return 0;
  }

  if (strncmp(r, "\\\\?\\", 4) == 0){
    memmove(r, r + 4, L - 4);
    r[L - 4] = '\0';
    return L - 4;
  }

  return L;
}
