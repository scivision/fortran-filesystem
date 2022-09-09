#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef _MSC_VER
#include <stdlib.h>
#include <direct.h>
#include <io.h>
#else
#include <unistd.h>
#endif

#ifdef _WIN32
#include <windows.h>
#endif

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

int get_maxp(){
return MAXP;
}

bool sys_posix() {
  char sep[2];

  filesep(sep);
  return sep[0] == '/';
}

void filesep(char* sep) {
#ifdef _WIN32
  strcpy(sep, "\\");
#else
  strcpy(sep, "/");
#endif
}

uintmax_t file_size(const char* path) {
  struct stat s;

  if (is_dir(path)) return 0;

  if (stat(path, &s) == 0) return s.st_size;

  return 0;
}


bool is_dir(const char* path){
  struct stat s;

  int i = stat(path, &s);

  // NOTE: root() e.g. "C:" needs a trailing slash
  return i == 0 && (s.st_mode & S_IFDIR);
}


bool is_exe(const char* path){
  struct stat s;

  if(stat(path, &s) != 0) return false;

#ifdef _MSC_VER
  return s.st_mode & _S_IEXEC;
#else
  return s.st_mode & S_IXUSR;
#endif
}


size_t root(const char* path, char* r) {

if (is_absolute(path)){

#ifdef _WIN32
  strncpy(r, &path[0], 2);
  r[2] = '\0';
#else
  strncpy(r, &path[0], 1);
  r[1] = '\0';
#endif

}
else {
  r = "";
}

return strlen(r);
}


bool is_absolute(const char* path){
  if(path == NULL) return false;

  size_t L = strlen(path);
  if(L < 1) return false;

#ifdef _WIN32
  if(L < 2) return false;
  if(path[1] != ':') return false;
  if(!isalpha(path[0])) return false;
  return true;
#endif

  return path[0] == '/';
}


bool is_symlink(const char* path){
  if(path==NULL) return false;

#ifdef _MSC_VER
  if(_access_s(path, 0 ) != 0) return false;
#else
  if(access(path, F_OK) != 0) return false;
#endif

#ifdef _WIN32
  return GetFileAttributes(path) & FILE_ATTRIBUTE_REPARSE_POINT;
#else
  struct stat buf;
  int p;

  if(lstat(path, &buf) != 0) return false;

  // return (buf.st_mode & S_IFMT) == S_IFLNK; // equivalent to below line
  return S_ISLNK(buf.st_mode);
#endif
}

int create_symlink(const char* target, const char* link) {

#ifdef _WIN32
  return !(CreateSymbolicLink(link, target, SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE) != 0);
#else
  symlink(target, link);
  // return value not supported on macOS
  return 0;
#endif

}


size_t get_cwd(char* path) {

#ifdef _MSC_VER
  if (_getcwd(path, _MAX_PATH) == NULL) return 0;
#else
  if (getcwd(path, PATH_MAX) == NULL) return 0;
#endif

  return strlen(path);
}


bool chmod_exe(const char* path){
  struct stat s;
  if(stat(path, &s) != 0) return false;

#ifdef _MSC_VER
  return _chmod(path, s.st_mode | _S_IEXEC) == 0;
#else
  return chmod(path, s.st_mode | S_IXUSR) == 0;
#endif
}

bool chmod_no_exe(const char* path){
  struct stat s;
  if(stat(path, &s) != 0) return false;

#ifdef _MSC_VER
  return _chmod(path, s.st_mode | !_S_IEXEC) == 0;
#else
  return chmod(path, s.st_mode | !S_IXUSR) == 0;
#endif
}


#ifdef _WIN32
extern void realpath(const char* path, char* rpath){
  _fullpath(rpath, path, _MAX_PATH);
}
#endif
