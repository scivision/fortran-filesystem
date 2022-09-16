#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#ifndef FS_DLL_NAME
#define FS_DLL_NAME NULL
#endif

#else

#ifdef HAVE_DLADDR
#include <dlfcn.h>
static void dl_dummy_func() {}
#endif

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

  if (!is_file(path)) return 0;

  if (stat(path, &s) == 0) return s.st_size;

  return 0;
}


bool is_dir(const char* path){
  struct stat s;

  int i = stat(path, &s);

  // NOTE: root() e.g. "C:" needs a trailing slash
  return i == 0 && (s.st_mode & S_IFDIR);
}


bool is_file(const char* path){
  struct stat s;

  int i = stat(path, &s);

  // NOTE: root() e.g. "C:" needs a trailing slash
  return i == 0 && (s.st_mode & S_IFREG);
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

size_t lib_path(char* path){

#ifdef _WIN32
  if (GetModuleFileName(GetModuleHandle(FS_DLL_NAME), path, MAX_PATH) != 0)
    return strlen(path);
#elif defined(HAVE_DLADDR)
  Dl_info info;

  if (dladdr( (void*)&dl_dummy_func, &info) != 0)
  {
    strcpy(path, info.dli_fname);
    return strlen(path);
  }
#endif


  return 0;

}


bool exists(const char* path) {
#ifdef _MSC_VER
  return _access_s(path, 0 ) == 0;
#else
  return access(path, F_OK) == 0;
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
  if(!exists(path)) return false;

#ifdef _WIN32
  return GetFileAttributes(path) & FILE_ATTRIBUTE_REPARSE_POINT;
#else
  struct stat buf;

  if(lstat(path, &buf) != 0) return false;

  // return (buf.st_mode & S_IFMT) == S_IFLNK; // equivalent to below line
  return S_ISLNK(buf.st_mode);
#endif
}

int create_symlink(const char* target, const char* link) {

#ifdef _WIN32
  if(is_dir(target)) {
    return !(CreateSymbolicLink(link, target,
      SYMBOLIC_LINK_FLAG_DIRECTORY | SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE) != 0);
  }
  else {
    return !(CreateSymbolicLink(link, target,
      SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE) != 0);
  }
#else
  return symlink(target, link);
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

bool fs_remove(const char* path) {
  if (!exists(path)) return true;

#ifdef _WIN32
  if (is_dir(path)){
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
