#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#else
#include <limits.h>
#include <unistd.h>
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

#ifdef _WIN32
  // NOTE: root() e.g. "C:" needs a trailing slash
  return i == 0 && (s.st_mode & S_IFDIR);
#else
  return i == 0 && S_ISDIR(s.st_mode);
#endif
}


size_t root(const char* path, char* r) {

if (is_absolute(path)){

#ifdef _WIN32
  memcpy(r, &path[0], 2);
  r[2] = '\0';
#else
  memcpy(r, &path[0], 1);
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


size_t get_cwd(char* path) {

#ifdef _WIN32
  if (_getcwd(path, _MAX_PATH) == NULL) return 0;
#else
  if (getcwd(path, PATH_MAX) == NULL) return 0;
#endif

  return strlen(path);

}

#ifdef _WIN32
extern void realpath(const char* path, char* rpath){
  _fullpath(rpath, path, _MAX_PATH);
}
#endif
