#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>  // IWYU pragma: keep

#ifdef _MSC_VER
#include <io.h>
#else
#include <unistd.h>
#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#include <sys/statvfs.h>
#endif

#include "ffilesystem.h"
#include <cwalk.h>

size_t fs_get_max_path(){ return FS_MAX_PATH; }

bool fs_cpp(){
// tell if fs core is C or C++
  return false;
}

size_t fs_compiler(char* name, size_t buffer_size)
{
  if(!name || buffer_size == 0){
    name = NULL;
    return 0;
  }

int L=0;

#if defined(__INTEL_LLVM_COMPILER)
  L = snprintf(name, buffer_size, "Intel LLVM %d %s", __INTEL_LLVM_COMPILER,  __VERSION__);
#elif defined(__NVCOMPILER_LLVM__)
  L = snprintf(name, buffer_size, "NVIDIA nvc %d.%d.%d", __NVCOMPILER_MAJOR__, __NVCOMPILER_MINOR__, __NVCOMPILER_PATCHLEVEL__);
#elif defined(__clang__)
  L = snprintf(name, buffer_size, "Clang %d.%d.%d", __clang_major__, __clang_minor__, __clang_patchlevel__);
#elif defined(__GNUC__)
  L = snprintf(name, buffer_size, "GNU GCC %d.%d.%d", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#elif defined(_MSC_VER)
  L = snprintf(name, buffer_size, "MSVC %d", _MSC_FULL_VER);
#else
  name[0] = '\0';
#endif

if (L < 0){
  fprintf(stderr, "ERROR:ffilesystem:fs_compiler: snprintf failed\n");
  L = 0;
}

  return L;
}

void fs_as_posix(char* path)
{
// force posix file seperator
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
  char s = '/';
  char *p = strchr(path, s);
  while (p) {
    *p = '\\';
    p = strchr(p+1, s);
  }
}


size_t fs_normal(const char* path, char* result, size_t buffer_size)
{
// normalize path
#ifdef _WIN32
  cwk_path_set_style(CWK_STYLE_WINDOWS);
#else
  cwk_path_set_style(CWK_STYLE_UNIX);
#endif
  size_t L = cwk_path_normalize(path, result, buffer_size);
  fs_as_posix(result);

if(FS_TRACE) printf("TRACE:normal in: %s  out: %s\n", path, result);

  return L;
}


size_t fs_file_name(const char* path, char* result, size_t buffer_size)
{
  if(strlen(path) == 0){
    result[0] = '\0';
    return 0;
  }

  const char *base;

if(FS_TRACE) printf("TRACE:file_name: %s\n", path);

#ifdef _WIN32
  cwk_path_set_style(CWK_STYLE_WINDOWS);
#else
  cwk_path_set_style(CWK_STYLE_UNIX);
#endif
  cwk_path_get_basename(path, &base, NULL);

if(FS_TRACE) printf("TRACE:file_name: %s => %s\n", path, base);

  strncpy(result, base, buffer_size);
  size_t L = strlen(result);
  result[L] = '\0';

  return L;
}


size_t fs_stem(const char* path, char* result, size_t buffer_size)
{
  char* buf = (char*) malloc(buffer_size);
  if(fs_file_name(path, buf, buffer_size) == 0){
    free(buf);
    result = NULL;
    return 0;
  }

  char* pos = strrchr(buf, '.');
  if (pos && pos != buf){
    strncpy(result, buf, pos-buf);
    result[pos-buf] = '\0';
  }
  else {
    strncpy(result, buf, buffer_size);
    result[strlen(result)] = '\0';
  }

  free(buf);
  return strlen(result);
}


size_t fs_join(const char* path, const char* other, char* result, size_t buffer_size)
{
  cwk_path_set_style(CWK_STYLE_UNIX);
  return cwk_path_join(path, other, result, buffer_size);
}


size_t fs_parent(const char* path, char* result, size_t buffer_size)
{
  char* buf = (char*) malloc(buffer_size);
  if(fs_normal(path, buf, buffer_size) == 0){
    free(buf);
    result = NULL;
    return 0;
  }

  size_t L;

  cwk_path_get_dirname(buf, &L);
  if(L == 0){
    free(buf);
    result[0] = '\0';
    return 0;
  }

  size_t M = min(L-1, buffer_size);
  strncpy(result, buf, M);
  free(buf);
  result[M] = '\0';

if(FS_TRACE) printf("TRACE: parent: %s => %s  %zu\n", path, result, M);
  return M;
}


size_t fs_suffix(const char* path, char* result, size_t buffer_size)
{
  char* buf = (char*) malloc(buffer_size);
  if(fs_file_name(path, buf, buffer_size) == 0){
    free(buf);
    result = NULL;
    return 0;
  }

  char* pos = strrchr(buf, '.');
  if (pos && pos != buf){
    strncpy(result, pos, buffer_size);
    result[strlen(result)] = '\0';
  }
  else {
    result[0] = '\0';
  }

  free(buf);

  return strlen(result);
}


size_t fs_with_suffix(const char* path, const char* suffix,
                      char* result, size_t buffer_size)
{
  if(strlen(suffix) == 0)
    return fs_stem(path, result, buffer_size);

  if(path[0] == '.'){
    // workaround for leading dot filename
    strncpy(result, path, buffer_size);
    result[strlen(result)] = '\0';
    strncat(result, suffix, buffer_size);
    return strlen(result);
  }

  cwk_path_set_style(CWK_STYLE_UNIX);
  cwk_path_change_extension(path, suffix, result, buffer_size);

  return fs_normal(result, result, buffer_size);
}


size_t fs_canonical(const char* path, bool strict, char* result, size_t buffer_size)
{
  // also expands ~

  if(strlen(path) == 0){
    result[0] = '\0';
    return 0;
  }

  if(strlen(path) == 1 && path[0] == '.')
    return fs_get_cwd(result, buffer_size);

  char* buf = (char*) malloc(buffer_size);
  if(fs_expanduser(path, buf, buffer_size) == 0){
    free(buf);
    goto retnull;
  }

  if(FS_TRACE) printf("TRACE:canonical in: %s  expanded: %s\n", path, buf);

  if(strict && !fs_exists(buf)) {
    fprintf(stderr, "ERROR:ffilesystem:canonical: %s => does not exist and strict=true\n", buf);
    free(buf);
    goto retnull;
  }

  char* buf2 = (char*) malloc(buffer_size);
#ifdef _WIN32
  char* t = _fullpath(buf2, buf, buffer_size);
#else
  char* t = realpath(buf, buf2);
#endif
  if (strict && t == NULL) {
    fprintf(stderr, "ERROR:ffilesystem:canonical: %s => %s\n", buf, strerror(errno));
    free(buf);
    free(buf2);
    goto retnull;
  }
  free(buf);

  size_t L = fs_normal(buf2, result, buffer_size);
  free(buf2);
  return L;

retnull:
  result = NULL;
  return 0;
}


size_t fs_relative_to(const char* to, const char* from, char* result, size_t buffer_size)
{
  if((strlen(to) == 0) || (strlen(from) == 0)){
    result[0] = '\0';
    return 0;
  }

  if(fs_is_absolute(to) != fs_is_absolute(from)){
    // cannot be relative, avoid bugs with MacOS
    result[0] = '\0';
    return 0;
  }

  if(strcmp(to, from) == 0){
    // short circuit if trivially equal
    result[0] = '.';
    result[1] = '\0';
    return 1;
  }

#ifdef _WIN32
  cwk_path_set_style(CWK_STYLE_WINDOWS);
#else
  cwk_path_set_style(CWK_STYLE_UNIX);
#endif
  cwk_path_get_relative(from, to, result, buffer_size);

  return fs_normal(result, result, buffer_size);
}


uintmax_t fs_file_size(const char* path)
{
  struct stat s;

  if (!fs_is_file(path))
    return 0;

  if (stat(path, &s) != 0)
    return 0;

  return s.st_size;;
}

uintmax_t fs_space_available(const char* path)
{
  // necessary for MinGW; seemed good choice for all platforms
  if(!fs_exists(path))
    return 0;

  char* r = (char*) malloc(FS_MAX_PATH);

  // for robustness and clarity, use root of path (necessary for Windows)
  if (!fs_root(path, r, FS_MAX_PATH))
    goto retzero;

#ifdef _WIN32
	DWORD ClusterSectors, SectorBytes, FreeClusters, TotalClusters;

  GetDiskFreeSpace(r, &ClusterSectors, &SectorBytes, &FreeClusters, &TotalClusters);
  free(r);

  return SectorBytes * ClusterSectors * FreeClusters;

#else
  struct statvfs stat;

  if (statvfs(r, &stat) != 0) {
    fprintf(stderr, "ERROR:ffilesystem:space_available: %s => %s\n", r, strerror(errno));
    goto retzero;
  }
  free(r);

  return stat.f_bsize * stat.f_bavail;
#endif

retzero:
  free(r);
  return 0;
}

bool fs_equivalent(const char* path1, const char* path2)
{
// both paths must exist, or they are not equivalent -- return false


  if(fs_is_reserved(path1) || fs_is_reserved(path2))
    return false;

  char* buf1 = (char*) malloc(FS_MAX_PATH);
  char* buf2 = (char*) malloc(FS_MAX_PATH);

  if((!fs_canonical(path1, true, buf1, FS_MAX_PATH) || !fs_canonical(path2, true, buf2, FS_MAX_PATH) ||
      fs_is_char_device(path1) || fs_is_char_device(path2)) ||
    !(fs_is_dir(buf1) || fs_is_dir(buf2) || fs_is_file(buf1) || fs_is_file(buf2))){

      free(buf1);
      free(buf2);
      return false;
  }

  bool eqv = strcmp(buf1, buf2) == 0;
  free(buf1);
  free(buf2);

  return eqv;

}


size_t fs_expanduser(const char* path, char* result, size_t buffer_size)
{
  if(path[0] != '~')
    return fs_normal(path, result, buffer_size);

  char* buf = (char*) malloc(buffer_size);
  if (!fs_get_homedir(buf, buffer_size)) {
    free(buf);
    return fs_normal(path, result, buffer_size);
  }

  // ~ alone
  size_t L = strlen(path);
  if (L < 3){
    L = fs_normal(buf, result, buffer_size);
    if(FS_TRACE) printf("TRACE:expanduser: orphan ~: homedir %s %s\n", buf, result);
    free(buf);
    return L;
  }

  strcat(buf, "/");

  if(FS_TRACE) printf("TRACE:expanduser: homedir %s\n", buf);

  strcat(buf, path+2);
  L = fs_normal(buf, result, buffer_size);
  if(FS_TRACE) printf("TRACE:expanduser result: %s\n", result);

  free(buf);

  return L;
}

bool fs_is_char_device(const char* path)
{
  // special POSIX file character device like /dev/null
  struct stat s;

  if(stat(path, &s) != 0)
    return false;

  // NOTE: root() e.g. "C:" needs a trailing slash
  return s.st_mode & S_IFCHR;
}


bool fs_is_dir(const char* path)
{
  struct stat s;

  if(stat(path, &s) != 0)
    return false;

  // NOTE: root() e.g. "C:" needs a trailing slash
  return s.st_mode & S_IFDIR;
}


bool fs_is_exe(const char* path)
{
  struct stat s;

  if(stat(path, &s) != 0)
    return false;

#ifdef _MSC_VER
  return s.st_mode & _S_IEXEC;
#else
  return s.st_mode & S_IXUSR;
#endif
}


bool fs_is_file(const char* path)
{
  if (fs_is_reserved(path))
    return false;

  struct stat s;

  if(stat(path, &s) != 0)
    return false;

  return s.st_mode & S_IFREG;
}


bool fs_is_reserved(const char* path)
{
#ifndef _WIN32
  return false;
#endif

  if(strcmp(path, "CON") == 0) return true;
  if(strcmp(path, "PRN") == 0) return true;
  if(strcmp(path, "AUX") == 0) return true;
  if(strcmp(path, "NUL") == 0) return true;
  if(strncmp(path, "COM", 3) == 0) return true;
  if(strncmp(path, "LPT", 3) == 0) return true;

  return false;
}

bool fs_exists(const char* path)
{
// false empty just for clarity
if(strlen(path) == 0)
  return false;

#ifdef _MSC_VER
  return _access_s(path, 0) == 0;
#else
  // <unistd.h>
  return access(path, F_OK) == 0;
#endif

}


size_t fs_root(const char* path, char* result, size_t buffer_size)
{
  size_t L;

#ifdef _WIN32
  cwk_path_set_style(CWK_STYLE_WINDOWS);
#else
  cwk_path_set_style(CWK_STYLE_UNIX);
#endif
  cwk_path_get_root(path, &L);

  size_t M = min(L, buffer_size);
  strncpy(result, path, M);
  result[M] = '\0';

if(FS_TRACE) printf("TRACE: root: %s => %s  %zu\n", path, result, M);
  return M;
}


bool fs_is_absolute(const char* path)
{
#ifdef _WIN32
  if (path[0] == '/')
    return false;
#endif

  return cwk_path_is_absolute(path);
}


bool fs_is_symlink(const char* path)
{
#ifdef _WIN32
  return fs_win32_is_symlink(path);
#else
  struct stat buf;

  if(lstat(path, &buf) != 0)
    return false;

  // return (buf.st_mode & S_IFMT) == S_IFLNK; // equivalent to below line
  return S_ISLNK(buf.st_mode);
#endif
}


int fs_create_symlink(const char* target, const char* link)
{
  if(!fs_exists(target)) {
    fprintf(stderr, "ERROR:filesystem:create_symlink: target path does not exist\n");
    return 1;
  }
  if(!link || strlen(link) == 0) {
    fprintf(stderr, "ERROR:filesystem:create_symlink: link path must not be empty\n");
    return 1;
  }

#ifdef _WIN32
  return fs_win32_create_symlink(target, link);
#else
  // <unistd.h>
  return symlink(target, link);
#endif
}


bool fs_remove(const char* path)
{
  if (!fs_exists(path))
    return false;

#ifdef _WIN32
  if (fs_is_dir(path)){
    // https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-removedirectorya
    return RemoveDirectory(path) != 0;
  }
  else {
    // https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-deletefilea
    return DeleteFile(path) != 0;
  }
#else
  return remove(path) == 0;
#endif
}


bool fs_chmod_exe(const char* path, bool executable)
{
  struct stat s;
  if(stat(path, &s) != 0)
    return false;
  if(s.st_mode & S_IFCHR)
    return false; // special POSIX file character device like /dev/null

#ifdef _MSC_VER
  return _chmod(path, s.st_mode | ((executable) ? _S_IEXEC : !_S_IEXEC) ) == 0;
#else
  return chmod(path, s.st_mode |  ((executable) ? S_IXUSR : !S_IXUSR) ) == 0;
#endif
// need parentheses to keep intended precedence
}


bool fs_touch(const char* path)
{
  if(strlen(path) == 0)
    return false;

  if (fs_exists(path))
    return fs_is_file(path);

  FILE* fid = fopen(path, "a");
  fclose(fid);

  return fs_is_file(path);
}


size_t fs_exe_dir(char* path, size_t buffer_size)
{
  char* buf = (char*) malloc(buffer_size);

  if(fs_exe_path(buf, buffer_size) == 0){
    free(buf);
    return 0;
  }

  size_t L = fs_parent(buf, path, buffer_size);

  free(buf);
  return L;
}

size_t fs_lib_dir(char* path, size_t buffer_size)
{
  char* buf = (char*) malloc(buffer_size);

  if(fs_lib_path(buf, buffer_size) == 0){
    fprintf(stderr, "ERROR:ffilesystem:fs_lib_dir: fs_lib_path failed\n");
    free(buf);
    return 0;
  }

  if(FS_TRACE) printf("TRACE:fs_lib_dir: %s %zu\n", buf, buffer_size);

  size_t L = fs_parent(buf, path, buffer_size);
  #ifdef __CYGWIN__
    if(!L){
      fprintf(stderr, "ERROR:ffilesystem:fs_lib_dir: fs_parent failed--known issue with Cygwin\n");
      free(buf);
      return 0;
    }
  #endif

  free(buf);
  return L;
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
  strncpy(result, buf2, buffer_size);
  result[L1] = '\0';
  free(buf);
  free(buf2);
  return L1;
}
