#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string.h>
#endif

size_t fs_win32_read_symlink(const char* path, char* r, size_t buffer_size)
{
  // https://stackoverflow.com/a/50182947
#ifdef _WIN32
  // get file handle
  HANDLE h = CreateFile(path, 0, 0, NULL, OPEN_EXISTING,
    FILE_FLAG_BACKUP_SEMANTICS, NULL);
  if(h == INVALID_HANDLE_VALUE)
    return 0;

  DWORD L = GetFinalPathNameByHandle(h, r, buffer_size, FILE_NAME_NORMALIZED);
  CloseHandle(h);
  if (L == 0)
    return 0;

  if (strncmp(r, "\\\\?\\", 4) == 0){
    memmove(r, r + 4, L - 4);
    r[L - 4] = '\0';
    return L - 4;
  }

  return L;
#else
  (void) path; (void) r; (void) buffer_size;
  return 0;
#endif
}
