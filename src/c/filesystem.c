#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <fileapi.h>
#include <io.h>
#include <direct.h> /* _mkdir */
#include <sys/utime.h>
#else
#include <pwd.h>  /* getpwuid */
#include <unistd.h>
#include <sys/statvfs.h>
#include <sys/time.h>
#endif

#if defined(_WIN32) && !defined(NOMINMAX)
#define NOMINMAX
#endif
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#if defined(__APPLE__) && defined(__MACH__)
#include "TargetConditionals.h"  /* TARGET_OS_MAC */
#endif

#if TARGET_OS_MAC
#include <copyfile.h>
#endif

#ifdef HAVE_UTSNAME_H
#include <sys/utsname.h>
#endif

#include "ffilesystem.h"
#include <cwalk.h>


size_t fs_get_max_path(){

#if defined(PATH_MAX)
  return PATH_MAX;
#elif defined (_MAX_PATH)
  return _MAX_PATH;
#elif defined (_POSIX_PATH_MAX)
  return _POSIX_PATH_MAX;
#else
  return 256;
#endif

 }

bool fs_cpp(){
// tell if fs core is C or C++
  return false;
}

long fs_lang(){
#ifdef __STDC_VERSION__
  return __STDC_VERSION__;
#else
  return 0L;
#endif
}

#ifndef _WIN32
static inline bool str_ends_with(const char *s, const char *suffix) {
  /* https://stackoverflow.com/a/41652727 */
    size_t slen = strlen(s);
    size_t suffix_len = strlen(suffix);

    return suffix_len <= slen && !strcmp(s + slen - suffix_len, suffix);
}
#endif

bool fs_is_admin(){
  // running as admin / root / superuser
#ifdef _WIN32
  fprintf(stderr, "ERROR:ffilesystem:fs_is_admin: not implemented for non-C++\n");
  return false;
#else
  return geteuid() == 0;
#endif
}

int fs_is_wsl() {
#ifdef HAVE_UTSNAME_H
  struct utsname buf;
  if (uname(&buf) != 0)
    return false;

  if (strcmp(buf.sysname, "Linux") != 0)
    return 0;
  if (str_ends_with(buf.release, "microsoft-standard-WSL2"))
    return 2;
  if (str_ends_with(buf.release, "-Microsoft"))
    return 1;
#endif

  return 0;
}


size_t fs_compiler(char* name, size_t buffer_size)
{
int L=0;

#if defined(__INTEL_LLVM_COMPILER)
  L = snprintf(name, buffer_size, "Intel LLVM %d %s", __INTEL_LLVM_COMPILER,  __VERSION__);
#elif defined(__NVCOMPILER_LLVM__)
  L = snprintf(name, buffer_size, "NVIDIA nvc %d.%d.%d", __NVCOMPILER_MAJOR__, __NVCOMPILER_MINOR__, __NVCOMPILER_PATCHLEVEL__);
#elif defined(__clang__)
  #ifdef __VERSION__
    L = snprintf(name, buffer_size, "Clang %s", __VERSION__);
  #else
    L = snprintf(name, buffer_size, "Clang %d.%d.%d", __clang_major__, __clang_minor__, __clang_patchlevel__);
  #endif
#elif defined(__GNUC__)
  L = snprintf(name, buffer_size, "GNU GCC %d.%d.%d", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#else
  name[0] = '\0';
#endif

if (L < 0){ // cppcheck-suppress knownConditionTrueFalse
  fprintf(stderr, "ERROR:ffilesystem:fs_compiler: snprintf failed\n");
  L = 0;
}
if((size_t)L >= buffer_size){  // cppcheck-suppress unsignedLessThanZero
  name[buffer_size-1] = '\0';
  L = buffer_size-1;
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
  cwk_path_set_style(fs_is_windows() ? CWK_STYLE_WINDOWS : CWK_STYLE_UNIX);

  size_t L = cwk_path_normalize(path, result, buffer_size);
  if(L > buffer_size){
    fprintf(stderr, "ERROR:ffilesystem: output buffer too small for string\n");
    return 0;
  }

  if(fs_is_windows())
    fs_as_posix(result);

  return L;
}


size_t fs_file_name(const char* path, char* result, size_t buffer_size)
{
  if(strlen(path) == 0){
    result[0] = '\0';
    return 0;
  }

  const char *base;

  cwk_path_set_style(fs_is_windows() ? CWK_STYLE_WINDOWS : CWK_STYLE_UNIX);

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
  if(!buf) return 0;
  if(!fs_file_name(path, buf, buffer_size)){
    free(buf);
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
  if(!buf) return 0;
  if(!fs_normal(path, buf, buffer_size)){
    free(buf);
    return 0;
  }

  size_t L;

  cwk_path_get_dirname(buf, &L);
  if(L == 0){
    free(buf);
    result[0] = '\0';
    return 0;
  }

  size_t M = min(L-1, buffer_size-1);
  strncpy(result, buf, M);
  free(buf);
  result[M] = '\0';

if(FS_TRACE) printf("TRACE: parent: %s => %s  %zu\n", path, result, M);
  return M;
}


size_t fs_suffix(const char* path, char* result, size_t buffer_size)
{
  char* buf = (char*) malloc(buffer_size);
  if(!buf) return 0;
  if(!fs_file_name(path, buf, buffer_size)){
    free(buf);
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

  cwk_path_set_style(fs_is_windows() ? CWK_STYLE_WINDOWS : CWK_STYLE_UNIX);
  cwk_path_change_extension(path, suffix, result, buffer_size);

  return fs_normal(result, result, buffer_size);
}


size_t fs_canonical(const char* path, bool strict, char* result, size_t buffer_size)
{
  // also expands ~
  // distinct from resolve()

  if(strlen(path) == 0){
    result[0] = '\0';
    return 0;
  }

  if(strlen(path) == 1 && path[0] == '.')
    return fs_get_cwd(result, buffer_size);

  char* buf = (char*) malloc(buffer_size);
  if(!buf) return 0;
  if(!fs_expanduser(path, buf, buffer_size)){
    free(buf);
    return 0;
  }

  if(FS_TRACE) printf("TRACE:canonical in: %s  expanded: %s  buffer_size %zu\n", path, buf, buffer_size);

  bool e = fs_exists(buf);
  size_t L;

  if(!e) {
    if(strict){
      fprintf(stderr, "ERROR:ffilesystem:canonical: %s => does not exist and strict=true\n", buf);
      free(buf);
      return 0;
    }
    else {
      L = fs_normal(buf, result, buffer_size);
      free(buf);
      return L;
    }
  }

  char* buf2 = (char*) malloc(buffer_size);
  if(!buf2) {
    free(buf);
    return 0;
  }

#ifdef _WIN32
  const char* t = _fullpath(buf2, buf, buffer_size);
#else
  const char* t = realpath(buf, buf2);
#endif

  if (!t) {
    fprintf(stderr, "ERROR:ffilesystem:canonical: %s   %s\n", buf, strerror(errno));
    free(buf);
    free(buf2);
    return 0;
  }
  free(buf);

  L = fs_normal(buf2, result, buffer_size);
  free(buf2);
  return L;
}


size_t fs_resolve(const char* path, bool strict, char* result, size_t buffer_size)
{
  // also expands ~
  // distinct from canonical()

  if(strlen(path) == 0 || (strlen(path) == 1 && path[0] == '.'))
    return fs_get_cwd(result, buffer_size);

  char* buf = (char*) malloc(buffer_size);
  if(!buf) return 0;
  if(!fs_expanduser(path, buf, buffer_size)){
    free(buf);
    return 0;
  }

  if(FS_TRACE) printf("TRACE:resolve: in: %s  expanded: %s  buffer_size %zu\n", path, buf, buffer_size);

  bool e = fs_exists(buf);
  size_t L;

  if(!e) {
    if(strict){
      fprintf(stderr, "ERROR:ffilesystem:resolve: %s => does not exist and strict=true\n", buf);
      free(buf);
      return 0;
    }
    else if (fs_is_absolute(buf)){
      L = fs_normal(buf, result, buffer_size);
      free(buf);
      return L;
    }
    else{
        if(!fs_join(".", buf, buf, buffer_size)){
        free(buf);
        return 0;
      }
    }
  }

  char* buf2 = (char*) malloc(buffer_size);
  if(!buf2) {
    free(buf);
    return 0;
  }

#ifdef _WIN32
  const char* t = _fullpath(buf2, buf, buffer_size);
#else
  const char* t = realpath(buf, buf2);
#endif

  if (!t && strict) {
    fprintf(stderr, "ERROR:ffilesystem:resolve: %s   %s\n", buf, strerror(errno));
    free(buf);
    free(buf2);
    return 0;
  }
  free(buf);

  L = fs_normal(buf2, result, buffer_size);
  free(buf2);
  return L;
}


bool fs_set_cwd(const char* path){

  if(strlen(path) == 0)
    return false;

  // <unistd.h>
  if(chdir(path) == 0)
    return true;

  fprintf(stderr, "ERROR:ffilesystem:set_cwd: %s    %s\n", path, strerror(errno));
  return false;

}


size_t fs_relative_to(const char* to, const char* from, char* result, size_t buffer_size)
{
  result[0] = '\0';
  if((strlen(to) == 0) || (strlen(from) == 0))
    return 0;

  /* cannot be relative, avoid bugs with MacOS */
  if(fs_is_absolute(to) != fs_is_absolute(from))
    return 0;

  cwk_path_set_style(fs_is_windows() ? CWK_STYLE_WINDOWS : CWK_STYLE_UNIX);

  cwk_path_get_relative(from, to, result, buffer_size);

  return fs_normal(result, result, buffer_size);
}


size_t fs_which(const char* name, char* result, size_t buffer_size)
{
  result[0] = '\0';
  if(strlen(name) == 0)
    return 0;

  if(fs_is_absolute(name) && fs_is_exe(name))
    return fs_normal(name, result, buffer_size);

  char* path = getenv("PATH");
  if(!path){
    fprintf(stderr, "ERROR:ffilesystem:which: PATH environment variable not set\n");
    return 0;
  }

  const char sep[2] = {fs_pathsep(), '\0'};

// strtok_r, strtok_s not necessarily available, and non-C++ is fallback
  char* p = strtok(path, sep);  // NOSONAR
  char* buf = (char*) malloc(buffer_size);
  if(!buf) return 0;

  while (p) {
    fs_join(p, name, buf, buffer_size);

    if(fs_is_exe(buf)){
      size_t L = fs_normal(buf, result, buffer_size);
      free(buf);
      return L;
    }
    p = strtok(NULL, sep);  // NOSONAR
  }

  free(buf);
  return 0;
}


uintmax_t fs_file_size(const char* path)
{
  struct stat s;

  if (fs_is_file(path) && !stat(path, &s))
    return s.st_size;
  return 0;
}

uintmax_t fs_space_available(const char* path)
{
#ifdef _WIN32
  (void) path;
  fprintf(stderr, "ERROR:ffilesystem:space_available: not implemented for non-C++\n");
  return 0;
#else
  // sanity check
  if(!fs_exists(path))
    return 0;

  const size_t m = fs_get_max_path();

  char* r = (char*) malloc(m);
  if(!r)
    return 0;

  // for robustness and clarity, use root of path
  if (!fs_root(path, r, m))
    goto retzero;

  struct statvfs stat;

  if (statvfs(r, &stat)) {
    fprintf(stderr, "ERROR:ffilesystem:space_available: %s => %s\n", r, strerror(errno));
    goto retzero;
  }
  free(r);

  return stat.f_bsize * stat.f_bavail;

retzero:
  free(r);
  return 0;
#endif
}

bool fs_equivalent(const char* path1, const char* path2)
{
// both paths must exist, or they are not equivalent -- return false

  const size_t m = fs_get_max_path();

  char* buf1 = (char*) malloc(m);
  if(!buf1) return false;
  char* buf2 = (char*) malloc(m);
  if(!buf2) {
    free(buf1);
    return false;
  }

  if((!fs_canonical(path1, true, buf1, m) || !fs_canonical(path2, true, buf2, m) ||
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
  if(path[0] != '~' || (strlen(path) > 1 && !(path[0] == '~' && path[1] == '/')))
    return fs_normal(path, result, buffer_size);

  char* buf = (char*) malloc(buffer_size);
  if(!buf) return 0;
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
// special character device like /dev/null
// Windows: https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fstat-fstat32-fstat64-fstati64-fstat32i64-fstat64i32
  struct stat s;
  return !stat(path, &s) && (s.st_mode & S_IFCHR);
  // S_ISCHR not available with MSVC
}


bool fs_is_dir(const char* path)
{
// NOTE: root() e.g. "C:" needs a trailing slash
  struct stat s;
  return !stat(path, &s) && (s.st_mode & S_IFDIR);
  // S_ISDIR not available with MSVC
}


bool fs_is_exe(const char* path)
{
  if (!fs_is_file(path))
    return false;

#ifdef _WIN32
  /* https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/access-s-waccess-s
  * in Windows, all readable files are executable.
  * Do not use _S_IEXEC, it is not reliable.
  */
  return fs_is_readable(path);
#else
  return !access(path, X_OK);
#endif
}

bool fs_is_readable(const char* path)
{
/* directory or file readable */

#ifdef _WIN32
  /* https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/access-s-waccess-s
  */
  return !_access_s(path, 4);
#else
  return !access(path, R_OK);
#endif
}

bool fs_is_writable(const char* path)
{
/* directory or file writable */

#ifdef _WIN32
  /* https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/access-s-waccess-s
  */
  return !_access_s(path, 2);
#else
  return !access(path, W_OK);
#endif
}


bool fs_is_file(const char* path)
{
  struct stat s;

  return !stat(path, &s) && (s.st_mode & S_IFREG);
  // S_ISREG not available with MSVC
}


bool fs_is_reserved(const char* path)
{
  fprintf(stderr, "ERROR:ffilesystem:is_reserved: not implemented without C++\n");
  (void)path;
  return false;
}

bool fs_exists(const char* path)
{
  /* fs_exists() is true even if path is non-readable
  * this is like Python pathlib.Path.exists()
  * but unlike kwSys:SystemTools:FileExists which uses R_OK instead of F_OK like this project.
  */
  // false empty just for clarity
  return strlen(path) &&
#ifdef _MSC_VER
  /* kwSys:SystemTools:FileExists is much more elaborate with Reparse point checks etc.
  * For this project, Windows non-C++ is not officially supported so we do it simply.
  * This way seems to work fine on Windows anyway.
  */
   !_access_s(path, 0);
#else
  // <unistd.h>
   !access(path, F_OK);
#endif
}


size_t fs_root(const char* path, char* result, size_t buffer_size)
{
  size_t L;

  cwk_path_set_style(fs_is_windows() ? CWK_STYLE_WINDOWS : CWK_STYLE_UNIX);

  cwk_path_get_root(path, &L);

  size_t M = min(L, buffer_size);
  strncpy(result, path, M);
  result[M] = '\0';

if(FS_TRACE) printf("TRACE: root: %s => %s  %zu\n", path, result, M);
  return M;
}


bool fs_is_absolute(const char* path)
{
  if(fs_is_windows() && path[0] == '/')
    return false;

  cwk_path_set_style(fs_is_windows() ? CWK_STYLE_WINDOWS : CWK_STYLE_UNIX);
  return cwk_path_is_absolute(path);
}


bool fs_is_symlink(const char* path)
{
#ifdef _WIN32
  DWORD a = GetFileAttributes(path);
  return (a != INVALID_FILE_ATTRIBUTES) && (a & FILE_ATTRIBUTE_REPARSE_POINT);
#else
  struct stat buf;

  return !lstat(path, &buf) && S_ISLNK(buf.st_mode);
  // return (buf.st_mode & S_IFMT) == S_IFLNK; // equivalent
#endif
}

size_t fs_read_symlink(const char* path, char* result, size_t buffer_size)
{
  if(!fs_is_symlink(path)){
    fprintf(stderr, "ERROR:ffilesystem:read_symlink: %s is not a symlink\n", path);
    return 0;
  }
#ifdef _WIN32
  (void) result;
  (void) buffer_size;
  fprintf(stderr, "ERROR:ffilesystem:read_symlink: not implemented for non-C++\n");
  return 0;
#else
  ssize_t L = readlink(path, result, buffer_size);
  if (L < 0){
    fprintf(stderr, "ERROR:ffilesystem:read_symlink: %s => %s\n", path, strerror(errno));
    return 0;
  }
  result[L] = '\0';
  return L;
#endif
}


bool fs_create_symlink(const char* target, const char* link)
{
  if(!fs_exists(target)) {
    fprintf(stderr, "ERROR:filesystem:create_symlink: target path does not exist\n");
    return false;
  }
  if(!link || strlen(link) == 0) {
    fprintf(stderr, "ERROR:filesystem:create_symlink: link path must not be empty\n");
    return false;
  }

#ifdef _WIN32
  fprintf(stderr, "ERROR:ffilesystem:create_symlink: not implemented for non-C++\n");
  return false;
#else
  // <unistd.h>
  return symlink(target, link) == 0;
#endif
}


bool fs_remove(const char* path)
{
  if (!fs_exists(path)){
    fprintf(stderr, "ERROR:ffilesystem:remove: %s does not exist\n", path);
    return false;
  }

#ifdef _WIN32
// https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-removedirectorya
// https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-deletefilea
  bool e = fs_is_dir(path) ? RemoveDirectoryA(path) : DeleteFileA(path);
  if (!e) {
    DWORD error = GetLastError();
    char *message;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		    NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (char *)&message, 0, NULL);
    fprintf(stderr, "ERROR:ffilesystem:remove: %s => %s\n", path, message);
  }
  return e;
#else
  return !remove(path);
#endif
}


bool fs_set_permissions(const char* path, int readable, int writable, int executable)
{
  struct stat s;
  if(stat(path, &s))
    return false;
  if(s.st_mode & S_IFCHR)
    return false; // special POSIX file character device like /dev/null

#ifdef _MSC_VER
  int m = s.st_mode;
  int r = _S_IREAD, w = _S_IWRITE, x = _S_IEXEC;
#else
  mode_t m = s.st_mode;
  mode_t r = S_IRUSR, w = S_IWUSR, x = S_IXUSR;
#endif

if(readable > 0)
  m |= r;
else if (readable < 0)
  m &= ~r;

if(writable > 0)
  m |= w;
else if (writable < 0)
  m &= ~w;

if(executable > 0)
  m |= x;
else if (executable < 0)
  m &= ~x;

return chmod(path, m) == 0;

}


size_t fs_get_permissions(const char* path, char* result, size_t buffer_size)
{
  if (buffer_size < 10) {
    fprintf(stderr, "ERROR:ffilesystem:fs_get_permissions: buffer_size must be at least 10\n");
    return 0;
  }

  struct stat s;

  if (stat(path, &s) != 0){
    fprintf(stderr, "ERROR:ffilesystem:fs_get_permissions: %s => %s\n", path, strerror(errno));
    return 0;
  }

#ifdef _MSC_VER
  (void) result;
  fprintf(stderr, "ERROR:ffilesystem:fs_get_permissions: not implemented for non-C++\n");
  return 0;
#else
  result[9] = '\0';
  result[0] = (s.st_mode & S_IRUSR) ? 'r' : '-';
  result[1] = (s.st_mode & S_IWUSR) ? 'w' : '-';
  result[2] = (s.st_mode & S_IXUSR) ? 'x' : '-';
  result[3] = (s.st_mode & S_IRGRP) ? 'r' : '-';
  result[4] = (s.st_mode & S_IWGRP) ? 'w' : '-';
  result[5] = (s.st_mode & S_IXGRP) ? 'x' : '-';
  result[6] = (s.st_mode & S_IROTH) ? 'r' : '-';
  result[7] = (s.st_mode & S_IWOTH) ? 'w' : '-';
  result[8] = (s.st_mode & S_IXOTH) ? 'x' : '-';
  return 9;
#endif
}


bool fs_touch(const char* path)
{
  if(strlen(path) == 0)
    return false;

  if (fs_exists(path) && !fs_is_file(path)){
    fprintf(stderr, "ERROR:ffilesystem:touch: %s exists but is not a file\n", path);
    return false;
  }

  if(fs_is_file(path) &&
#ifdef _WIN32
    // https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/utime-utime32-utime64-wutime-wutime32-wutime64
    _utime(path, NULL)){
#else
    utimes(path, NULL)){
#endif
    fprintf(stderr, "ERROR:ffilesystem:touch: %s => %s\n", path, strerror(errno));
    return false;
  }

  FILE* fid = fopen(path, "a+b");
  if(!fid){
    fprintf(stderr, "ERROR:ffilesystem:touch: %s => %s\n", path, strerror(errno));
    return false;
  }
  if(fclose(fid)){
    fprintf(stderr, "ERROR:ffilesystem:touch: %s => %s\n", path, strerror(errno));
    return false;
  }

  return fs_is_file(path);
}

bool fs_is_subdir(const char* subdir, const char* dir)
{
  // is subdir a subdirectory of dir -- lexical operation
  const size_t m = fs_get_max_path();

  char* buf1 = (char*) malloc(m);
  if(!buf1) return false;
  char* buf2 = (char*) malloc(m);
  if(!buf2) {
    free(buf1);
    return false;
  }

  size_t Ls = fs_normal(subdir, buf1, m);
  size_t Ld = fs_normal(dir, buf2, m);

  bool yes = Ls > Ld && strncmp(buf1, buf2, Ld) == 0;

  free(buf1);
  free(buf2);

  return yes;

}


size_t fs_make_absolute(const char* path, const char* base,
                        char* result, size_t buffer_size)
{
  size_t L1 = fs_expanduser(path, result, buffer_size);

  if (fs_is_absolute(result))
    return L1;

  char* buf = (char*) malloc(buffer_size);
  if(!buf) return 0;
  size_t L2 = fs_expanduser(base, buf, buffer_size);
  if(L2 == 0){
    free(buf);
    return L1;
  }

  char* buf2 = (char*) malloc(buffer_size);
  if(!buf2){
    free(buf);
    return 0;
  }
  L1 = fs_join(buf, result, buf2, buffer_size);
  strncpy(result, buf2, buffer_size);
  result[L1] = '\0';
  free(buf);
  free(buf2);
  return L1;
}


size_t fs_make_tempdir(char* result, size_t buffer_size)
{
#ifdef _WIN32
  (void) result; (void) buffer_size;
  fprintf(stderr, "ERROR:ffilesystem:fs_make_tempdir: not implemented for non-C++\n");
  return 0;
#else
  char tmpl[] = "tmp.XXXXXX";

  char* tmp = mkdtemp(tmpl);
  /* Linux: stdlib.h  macOS: unistd.h */
  if (!tmp){
    fprintf(stderr, "ERROR:filesystem:fs_make_tempdir:mkdtemp: could not create temporary directory %s\n", strerror(errno));
    return 0;
  }

  return fs_normal(tmp, result, buffer_size);
#endif
}


size_t fs_long2short(const char* in, char* out, size_t buffer_size){
#ifdef _WIN32
  (void) in; (void) out; (void) buffer_size;
  fprintf(stderr, "ERROR:ffilesystem:fs_long2short: not implemented for non-C++\n");
  return 0;
#else
  fprintf(stderr, "ERROR:ffilesystem:fs_long2short: windows-only\n");
  strncpy(out, in, buffer_size);
  return strlen(out);
#endif
}

size_t fs_short2long(const char* in, char* out, size_t buffer_size){
#ifdef _WIN32
  (void) in; (void) out; (void) buffer_size;
  fprintf(stderr, "ERROR:ffilesystem:fs_short2long: not implemented for non-C++\n");
  return 0;
#else
  fprintf(stderr, "ERROR:ffilesystem:fs_short2long: windows-only\n");
  strncpy(out, in, buffer_size);
  return strlen(out);
#endif
}

/* environment variable functions */


static size_t fs_getenv(const char* name, char* path, size_t buffer_size)
{
  // <stdlib.h>
  char* buf = getenv(name);
  if(!buf) // not error because sometimes we just check if envvar is defined
    return 0;
  else if(strlen(buf) >= buffer_size){
    fprintf(stderr, "ERROR:ffilesystem:fs_getenv: buffer_size %zu is too small for %s\n", buffer_size, name);
    return 0;
  }

  return fs_normal(buf, path, buffer_size);
}


size_t fs_get_cwd(char* path, size_t buffer_size)
{
// <direct.h> https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/getcwd-wgetcwd?view=msvc-170
// <unistd.h> https://www.man7.org/linux/man-pages/man3/getcwd.3.html
#ifdef _WIN32
  char* x = _getcwd(path, buffer_size);
#else
  char* x = getcwd(path, buffer_size);
#endif

  if(!x) {
    fprintf(stderr, "ERROR:ffilesystem:fs_get_cwd: %s\n", strerror(errno));
    return 0;
  }

  if(FS_TRACE) printf("TRACE:fs_get_cwd: %s  %s   buffer_size %zu  strlen %zu\n", x, path, buffer_size, strlen(path));

  return fs_normal(path, path, buffer_size);
}

size_t fs_get_homedir(char* path, size_t buffer_size)
{
  size_t L = fs_getenv(fs_is_windows() ?  "USERPROFILE" : "HOME", path, buffer_size);
  if (L)
    return L;

#ifdef _WIN32
  return 0;
#else
  const char *h = getpwuid(geteuid())->pw_dir;
  if (!h)
    return 0;

  return fs_normal(h, path, buffer_size);
#endif
}

size_t fs_get_tempdir(char* path, size_t buffer_size)
{
  size_t L = fs_getenv(fs_is_windows() ? "TEMP" : "TMPDIR", path, buffer_size);
  if(L)
    return L;

  if (buffer_size > 4 && fs_is_dir("/tmp")){
    strcpy(path, "/tmp");
    return 4;
  }

  return 0;
}



bool fs_copy_file(const char* source, const char* dest, bool overwrite) {
  if(!fs_is_file(source)) {
    fprintf(stderr,"ERROR:ffilesystem:copy_file: source file does not exist %s\n", source);
    return false;
  }
  if(strlen(dest) == 0) {
    fprintf(stderr, "ERROR:ffilesystem:copy_file: destination path must not be empty\n");
    return false;
  }

  if(fs_exists(dest)){
    if(fs_is_file(dest)){
      if(overwrite){
        if(!fs_remove(dest))
          fprintf(stderr, "ERROR:ffilesystem:copy_file: could not remove existing destination file %s\n", dest);
      } else {
        fprintf(stderr, "ERROR:ffilesystem:copy_file: destination file exists but overwrite=false %s\n", dest);
        return false;
      }
    } else {
      fprintf(stderr, "ERROR:ffilesystem:copy_file: destination path exists %s\n", dest);
      return false;
    }
  }


#if defined(_WIN32)
    if(!CopyFile(source, dest, true)){
      fprintf(stderr, "ERROR:ffilesystem:copy_file: could not copy file %s to %s\n", source, dest);
      return false;
    }
#elif TARGET_OS_MAC
  /* copy-on-write file
  * based on kwSys:SystemTools:CloneFileContent
  * https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man3/copyfile.3.html
  * COPYFILE_CLONE is a 'best try' flag, which falls back to a copy if the clone fails.
  */
  if(copyfile(source, dest, NULL, COPYFILE_METADATA | COPYFILE_CLONE) < 0){
    fprintf(stderr, "ERROR:ffilesystem:copy_file: could not clone file %s to %s\n", source, dest);
    return false;
  }
#else
    // https://stackoverflow.com/a/29082484
    const int bufferSize = 4096;
    char buf[bufferSize];
    FILE *rid = fopen(source, "r");
    FILE *wid = fopen(dest, "w");

    if (rid == NULL || wid == NULL) {
      fprintf(stderr, "ERROR:ffilesystem:copy_file: could not open file %s or %s\n", source, dest);
      return false;
    }

    while (!feof(rid)) {
      size_t bytes = fread(buf, 1, sizeof(buf), rid);
      if (bytes)
        fwrite(buf, 1, bytes, wid);
    }

    fclose(rid);
    fclose(wid);
#endif

  return fs_is_file(dest);

}


bool fs_create_directories(const char* path) {

  if(strlen(path) == 0) {
    fprintf(stderr, "ERROR:ffilesystem:create_directories: path must not be empty\n");
    return false;
  }

  if(fs_exists(path)){
    if(fs_is_dir(path))
      return true;

    fprintf(stderr, "ERROR:filesystem:create_directories: %s already exists but is not a directory\n", path);
    return false;
  }

  const size_t m = fs_get_max_path();

  // To disambiguate, use an absolute path -- must resolve multiple times because realpath only gives one level of non-existant path
  char* buf = (char*) malloc(m);
  if(!buf) return false;

  size_t L = fs_resolve(path, false, buf, m);
  if(L == 0){
    free(buf);
    return false;
  }

  if (FS_TRACE) printf("TRACE: mkdir %s resolved => %s\n", path, buf);

// use mkdir() building up directory components using strtok()
// strtok_r, strtok_s not necessarily available, and non-C++ is fallback
mkdir_loop: ;
  char* q = strtok(buf, "/");  // NOSONAR
  char* dir = (char*) malloc(L + 2);
  // + 2 to account for \0 and leading /
  if (!dir) {
    free(buf);
    return false;
  }

  dir[1] = '\0';
  dir[0] = (fs_is_windows()) ? '\0' : '/';

  while (q) {
    strcat(dir, q);
    if (FS_TRACE) printf("TRACE: mkdir %s\n", dir);

    if (
#ifdef _WIN32
      _mkdir(dir)
#else
      mkdir(dir, S_IRWXU)
#endif
        && errno != EEXIST) {
      fprintf(stderr, "ERROR:ffilesystem:create_directories: %s %s => %s\n", buf, dir, strerror(errno));
      free(buf);
      free(dir);
      return false;
    }
    strcat(dir, "/");
    q = strtok(NULL, "/"); // NOSONAR
  }
  /* check that path was adequately resolved and created */
  size_t L1 = fs_resolve(path, false, buf, m);
  if(L1 != L){
    if (FS_TRACE) printf("TRACE: mkdir %s iteration resolved => %s\n", path, buf);
    L = L1;
    free(dir);
    goto mkdir_loop;
  }

  free(dir);

  bool ok = fs_is_dir(buf);
  free(buf);
  return ok;
}


/* stubs for non-implemented functions */

size_t fs_exe_dir(char* path, size_t buffer_size)
{
  fprintf(stderr, "ERROR:ffilesystem:fs_exe_dir: not implemented for non-C++\n");
  (void) path; (void) buffer_size;
  return 0;
}

size_t fs_lib_dir(char* path, size_t buffer_size)
{
  fprintf(stderr, "ERROR:ffilesystem:fs_lib_dir: not implemented for non-C++\n");
  (void) path; (void) buffer_size;
  return 0;
}


size_t fs_exe_path(char* path, size_t buffer_size)
{
  (void) path; (void) buffer_size;
  fprintf(stderr, "ERROR:ffilesystem:fs_exe_path: not implemented for non-C++\n");
  return 0;
}

size_t fs_lib_path(char* path, size_t buffer_size)
{
  (void) path; (void) buffer_size;
  fprintf(stderr, "ERROR:ffilesystem:fs_lib_path: not implemented for non-C++\n");
  return 0;
}
