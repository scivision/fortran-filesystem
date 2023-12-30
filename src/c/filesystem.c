#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#include <unistd.h>

#include <sys/statvfs.h>

#ifdef HAVE_UTSNAME_H
#include <sys/utsname.h>
#endif

#include "ffilesystem.h"
#include <cwalk.h>

size_t fs_get_max_path(){ return FS_MAX_PATH; }

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


static bool str_ends_with(const char *s, const char *suffix) {
  /* https://stackoverflow.com/a/41652727 */
    size_t slen = strlen(s);
    size_t suffix_len = strlen(suffix);

    return suffix_len <= slen && !strcmp(s + slen - suffix_len, suffix);
}

bool fs_is_admin(){
  // running as admin / root / superuser
  return geteuid() == 0;
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
  cwk_path_set_style(CWK_STYLE_UNIX);

  size_t L = cwk_path_normalize(path, result, buffer_size);
  if(L > buffer_size){
    fprintf(stderr, "ERROR:ffilesystem: output buffer too small for string\n");
    return 0;
  }

//if(FS_TRACE) printf("TRACE:normal in: %s  out: %s\n", path, result);

  return L;
}


size_t fs_file_name(const char* path, char* result, size_t buffer_size)
{
  if(strlen(path) == 0){
    result[0] = '\0';
    return 0;
  }

  const char *base;

  cwk_path_set_style(CWK_STYLE_UNIX);

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

  cwk_path_set_style(CWK_STYLE_UNIX);
  cwk_path_change_extension(path, suffix, result, buffer_size);

  return fs_normal(result, result, buffer_size);
}


size_t fs_canonical(const char* path, bool strict, char* result, size_t buffer_size)
{
  // also expands ~
  // distinct from resolve

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

  const char* t = realpath(buf, buf2);

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
  // distinct from resolve

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

  const char* t = realpath(buf, buf2);

  if (!t) {
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

  if(strcmp(to, from) == 0){
    // short circuit if trivially equal
    result[0] = '.';
    result[1] = '\0';
    return 1;
  }

  cwk_path_set_style(CWK_STYLE_UNIX);

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

  char* p = strtok(path, ":");
  char* buf = (char*) malloc(buffer_size);
  if(!buf) return 0;

  while (p) {
    fs_join(p, name, buf, buffer_size);

    if(fs_is_exe(buf)){
      size_t L = fs_normal(buf, result, buffer_size);
      free(buf);
      return L;
    }
    p = strtok(NULL, ":");
  }

  free(buf);
  return 0;
}


uintmax_t fs_file_size(const char* path)
{
  struct stat s;

  return !fs_is_file(path) ? 0
    : stat(path, &s) ? 0
    : s.st_size;
}

uintmax_t fs_space_available(const char* path)
{
  // necessary for MinGW; seemed good choice for all platforms
  if(!fs_exists(path))
    return 0;

  char* r = (char*) malloc(FS_MAX_PATH);
  if(!r)
    return 0;

  // for robustness and clarity, use root of path (necessary for Windows)
  if (!fs_root(path, r, FS_MAX_PATH))
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
}

bool fs_equivalent(const char* path1, const char* path2)
{
// both paths must exist, or they are not equivalent -- return false

  char* buf1 = (char*) malloc(FS_MAX_PATH);
  if(!buf1) return false;
  char* buf2 = (char*) malloc(FS_MAX_PATH);
  if(!buf2) {
    free(buf1);
    return false;
  }

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
// special POSIX file character device like /dev/null
// NOTE: root() e.g. "C:" needs a trailing slash
  struct stat s;

  return stat(path, &s) ? false : s.st_mode & S_IFCHR;
}


bool fs_is_dir(const char* path)
{
// NOTE: root() e.g. "C:" needs a trailing slash
  struct stat s;

  return stat(path, &s) ? false : s.st_mode & S_IFDIR;
}


bool fs_is_exe(const char* path)
{
  struct stat s;

  return stat(path, &s) ? false : s.st_mode & S_IXUSR;
}


bool fs_is_file(const char* path)
{
  struct stat s;

  return stat(path, &s) ? false : s.st_mode & S_IFREG;
}


bool fs_is_reserved(const char* path)
{
  // non-c++ has no windows support
  (void)path;
  return false;
}

bool fs_exists(const char* path)
{
  // false empty just for clarity
  return strlen(path) == 0 ? false : !access(path, F_OK);
}


size_t fs_root(const char* path, char* result, size_t buffer_size)
{
  size_t L;

  cwk_path_set_style(CWK_STYLE_UNIX);

  cwk_path_get_root(path, &L);

  size_t M = min(L, buffer_size);
  strncpy(result, path, M);
  result[M] = '\0';

if(FS_TRACE) printf("TRACE: root: %s => %s  %zu\n", path, result, M);
  return M;
}


bool fs_is_absolute(const char* path)
{
  cwk_path_set_style(CWK_STYLE_UNIX);
  return cwk_path_is_absolute(path);
}


bool fs_is_symlink(const char* path)
{
  struct stat buf;

  return lstat(path, &buf) ? false : S_ISLNK(buf.st_mode);
  // return (buf.st_mode & S_IFMT) == S_IFLNK; // equivalent
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

  // <unistd.h>
  return symlink(target, link);
}


bool fs_remove(const char* path)
{
  if (!fs_exists(path))
    return false;

  return !remove(path);
}


bool fs_chmod_exe(const char* path, bool executable)
{
  struct stat s;
  if(stat(path, &s))
    return false;
  if(s.st_mode & S_IFCHR)
    return false; // special POSIX file character device like /dev/null

return chmod(path, s.st_mode |  ((executable) ? S_IXUSR : !S_IXUSR) ) == 0;
// need parentheses to keep intended precedence
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
  if(!buf) return 0;

  if(!fs_exe_path(buf, buffer_size)){
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
  if(!buf) return 0;

  if(!fs_lib_path(buf, buffer_size)){
    fprintf(stderr, "ERROR:ffilesystem:fs_lib_dir: fs_lib_path failed\n");
    free(buf);
    return 0;
  }

  if(FS_TRACE) printf("TRACE:fs_lib_dir: %s %zu\n", buf, buffer_size);

  size_t L = fs_parent(buf, path, buffer_size);

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
  if(!buf) return 0;
  size_t L2 = fs_expanduser(top_path, buf, buffer_size);
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
  char tmpl[] = "tmp.XXXXXX";

  char* tmp = mkdtemp(tmpl);
  /* Linux: stdlib.h  macOS: unistd.h */
  if (!tmp){
    fprintf(stderr, "ERROR:filesystem:fs_make_tempdir:mkdtemp: could not create temporary directory %s\n", strerror(errno));
    return 0;
  }

  return fs_normal(tmp, result, buffer_size);
}
