#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#ifdef _MSC_VER
#include <direct.h>
#include <io.h>
#else
#include <unistd.h>
#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Shlwapi.h"
#endif


#include "ffilesystem.h"
#include "cwalk.h"

#define TRACE 0


char* as_windows(const char*);

size_t fs_filesep(char* sep) {
#ifdef _WIN32
  strcpy(sep, "\\");
#else
  strcpy(sep, "/");
#endif

  return strlen(sep);
}


size_t fs_normal(const char* path, char* normalized, size_t buffer_size) {
  if(path == NULL)
    return 0;

  cwk_path_set_style(CWK_STYLE_UNIX);
  size_t L = cwk_path_normalize(path, normalized, buffer_size);

  if(TRACE) printf("TRACE:normal in: %s  out: %s\n", path, normalized);

// force posix file seperator
  char s='\\';
  char *p = strchr(normalized, s);
  while (p) {
      *p = '/';
      p = strchr(p+1, s);
  }

  return L;

}


size_t fs_file_name(const char* path, char* result, size_t buffer_size){

  if(path == NULL || strlen(path) == 0)
    return 0;

  const char *base;

  if(TRACE) printf("TRACE:file_name: %s\n", path);

  cwk_path_set_style(CWK_STYLE_UNIX);
  cwk_path_get_basename(path, &base, NULL);

  if(TRACE) printf("TRACE:file_name: %s => %s\n", path, base);

  strncpy(result, base, buffer_size);
  size_t L = strlen(result);
  result[L] = '\0';

  return L;
}


size_t fs_stem(const char* path, char* result, size_t buffer_size){

  if(path == NULL || strlen(path) == 0)
    return 0;

  char* buf = (char*) malloc(buffer_size);
  fs_file_name(path, buf, buffer_size);

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


size_t fs_join(const char* path, const char* other, char* result, size_t buffer_size){
  if(path == NULL || other == NULL)
    return 0;

  cwk_path_set_style(CWK_STYLE_UNIX);
  return cwk_path_join(path, other, result, buffer_size);
}


size_t fs_parent(const char* path, char* result, size_t buffer_size){

  if(path == NULL || strlen(path) == 0) {
    strcpy(result, ".");
    return 1;
  }

  char* buf = (char*) malloc(buffer_size);
  if(fs_normal(path, buf, buffer_size) == 0){
    free(buf);
    return 0;
  }

  char* pos = strrchr(buf, '/');
  if (pos){
    strncpy(result, buf, pos-buf);
    result[pos-buf] = '\0';
  }
  else {
    strcpy(result, ".");
  }

  free(buf);
  return strlen(result);
}


size_t fs_suffix(const char* path, char* result, size_t buffer_size){

  if(path == NULL || strlen(path) == 0)
    return 0;

  char* buf = (char*) malloc(buffer_size);
  fs_file_name(path, buf, buffer_size);

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


size_t fs_with_suffix(const char* path, const char* suffix, char* result, size_t buffer_size){
  if(path == NULL || suffix == NULL)
    return 0;

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
  return cwk_path_change_extension(path, suffix, result, buffer_size);
}


size_t canonical(const char* path, bool strict, char* result, size_t buffer_size) {
  // also expands ~

  if( path == NULL || strlen(path) == 0 )
    return 0;

  if(strlen(path) == 1 && path[0] == '.')
    return fs_get_cwd(result, buffer_size);

  char* buf = (char*) malloc(buffer_size);
  if(fs_expanduser(path, buf, buffer_size) == 0){
    free(buf);
    return 0;
  }

  if(TRACE) printf("TRACE:canonical in: %s  expanded: %s\n", path, buf);

  if(strict && !fs_exists(buf)) {
    free(buf);
    return 0;
  }

char* buf2 = (char*) malloc(buffer_size);
#ifdef _WIN32
  char* t = _fullpath(buf2, buf, buffer_size);
#else
  char* t = realpath(buf, buf2);
#endif
  if (strict && t == NULL) {
    fprintf(stderr, "ERROR:canonical: %s => %s\n", buf, strerror(errno));
    free(buf);
    free(buf2);
    return 0;
  }

  size_t L = fs_normal(buf2, result, buffer_size);
  free(buf);
  free(buf2);
  return L;
}


size_t relative_to(const char* to, const char* from, char* result, size_t buffer_size) {

  // undefined case, avoid bugs with MacOS
  if( to == NULL || (strlen(to) == 0) || from ==NULL || (strlen(from) == 0) )
    return 0;

  // cannot be relative, avoid bugs with MacOS
  if(fs_is_absolute(to) != fs_is_absolute(from))
    return 0;

  // short circuit if trivially equal
  if(strcmp(to, from) == 0){
    strcpy(result, ".");
    return strlen(result);
  }

  cwk_path_set_style(CWK_STYLE_UNIX);
  return cwk_path_get_relative(from, to, result, buffer_size);
}


uintmax_t fs_file_size(const char* path) {
  struct stat s;

  if (!fs_is_file(path))
    return 0;

  if (stat(path, &s) != 0)
    return 0;

  return s.st_size;;
}

bool equivalent(const char* path1, const char* path2){
// this is for exisitng paths

  char* buf1 = (char*) malloc(MAXP);
  char* buf2 = (char*) malloc(MAXP);

  canonical(path1, true, buf1, MAXP);
  canonical(path2, true, buf2, MAXP);

  bool eqv = (strlen(buf1) > 0) && (strlen(buf2) > 0) && strcmp(buf1, buf2) == 0;
  free(buf1);
  free(buf2);
  return eqv;

}

size_t fs_expanduser(const char* path, char* result, size_t buffer_size){

  if(path == NULL)
    return 0;

  size_t L = strlen(path);
  if(L == 0)
    return L;

  if(path[0] != '~')
    return fs_normal(path, result, buffer_size);

  char* buf = (char*) malloc(buffer_size);
  if (!fs_get_homedir(buf, buffer_size)) {
    free(buf);
    return fs_normal(path, result, buffer_size);
  }

  // ~ alone
  if (L < 3){
    L = fs_normal(buf, result, buffer_size);
    if(TRACE) printf("TRACE:expanduser: orphan ~: homedir %s %s\n", buf, result);
    free(buf);
    return L;
  }

  strcat(buf, "/");

  if(TRACE) printf("TRACE:expanduser: homedir %s\n", buf);

  strcat(buf, path+2);
  L = fs_normal(buf, result, buffer_size);
  if(TRACE) printf("TRACE:expanduser result: %s\n", result);

  free(buf);
  return L;
}

size_t fs_get_homedir(char* result, size_t buffer_size) {

char* buf;
size_t L;

#ifdef _WIN32
  buf = (char*) malloc(buffer_size);
  if(getenv_s(&L, buf, buffer_size, "USERPROFILE") != 0){
    fprintf(stderr, "ERROR:get_homedir: %s\n", strerror(errno));
    free(buf);
    return 0;
  }
#else
  buf = getenv("HOME");
#endif
  L = fs_normal(buf, result, buffer_size);
  if(TRACE) printf("TRACE: get_homedir: %s %s\n", buf, result);
#ifdef _WIN32
  free(buf);
#endif
  return L;
}

size_t fs_get_tempdir(char* result, size_t buffer_size) {

char* buf;
size_t L;

#ifdef _WIN32
  buf = (char*) malloc(buffer_size);
  if(GetTempPath((DWORD)buffer_size, buf) == 0){
    fprintf(stderr, "ERROR:get_tempdir: %s\n", strerror(errno));
    free(buf);
    return 0;
  }
#else
  buf = getenv("TMPDIR");
#endif

  L = fs_normal(buf, result, buffer_size);
#ifdef _WIN32
  free(buf);
#endif
  return L;
}


bool fs_is_dir(const char* path){
  struct stat s;

  int i = stat(path, &s);

  // NOTE: root() e.g. "C:" needs a trailing slash
  return i == 0 && (s.st_mode & S_IFDIR);
}


bool fs_is_exe(const char* path){
  struct stat s;

  if(stat(path, &s) != 0) return false;

#ifdef _MSC_VER
  return s.st_mode & _S_IEXEC;
#else
  return s.st_mode & S_IXUSR;
#endif
}


bool fs_is_file(const char* path){
  struct stat s;

  int i = stat(path, &s);

  // NOTE: root() e.g. "C:" needs a trailing slash
  return i == 0 && (s.st_mode & S_IFREG);
}


bool fs_exists(const char* path) {
// false empty just for clarity
if(path == NULL || strlen(path) == 0)
  return false;

#ifdef _MSC_VER
  return _access_s(path, 0) == 0;
#else
  return access(path, F_OK) == 0;
#endif

}


size_t fs_root(const char* path, char* result, size_t buffer_size) {

if (fs_is_absolute(path)){

#ifdef _WIN32
  strncpy(result, &path[0], buffer_size);
  result[2] = '\0';
#else
  strncpy(result, &path[0], buffer_size);
  result[1] = '\0';
#endif

}
else {
  result = "";
}

  return strlen(result);
}


bool fs_is_absolute(const char* path){
  if(path == NULL)
    return false;

  size_t L = strlen(path);
  if(L < 1)
    return false;

#ifdef _WIN32
  if(L < 2)
    return false;
  if(path[1] != ':')
    return false;
  if(!isalpha(path[0]))
    return false;
  return true;
#endif

  return path[0] == '/';
}


bool fs_is_symlink(const char* path){
  if(path==NULL)
    return false;
  if(!fs_exists(path))
    return false;

#ifdef _WIN32
  return GetFileAttributes(path) & FILE_ATTRIBUTE_REPARSE_POINT;
#else
  struct stat buf;

  if(lstat(path, &buf) != 0)
    return false;

  // return (buf.st_mode & S_IFMT) == S_IFLNK; // equivalent to below line
  return S_ISLNK(buf.st_mode);
#endif
}


int fs_create_symlink(const char* target, const char* link) {

#ifdef _WIN32
  if(fs_is_dir(target)) {
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


size_t fs_get_cwd(char* path, size_t buffer_size) {

#ifdef _MSC_VER
// https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/getcwd-wgetcwd?view=msvc-170
  if (_getcwd(path, (DWORD)buffer_size) == NULL)
    return 0;
#else
  if (getcwd(path, buffer_size) == NULL)
    return 0;
#endif

  return fs_normal(path, path, buffer_size);
}

bool fs_remove(const char* path) {
  if (!fs_exists(path))
    return true;

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

bool fs_chmod_exe(const char* path){
  struct stat s;
  if(stat(path, &s) != 0)
    return false;

#ifdef _MSC_VER
  return _chmod(path, s.st_mode | _S_IEXEC) == 0;
#else
  return chmod(path, s.st_mode | S_IXUSR) == 0;
#endif
}

bool fs_chmod_no_exe(const char* path){
  struct stat s;
  if(stat(path, &s) != 0)
    return false;

#ifdef _MSC_VER
  return _chmod(path, s.st_mode | !_S_IEXEC) == 0;
#else
  return chmod(path, s.st_mode | !S_IXUSR) == 0;
#endif
}

bool touch(const char* path) {

  if (fs_exists(path) && !fs_is_file(path))
    return false;

  if(!fs_is_file(path)) {
    FILE* fid = fopen(path, "a");
    fclose(fid);
  }

  return fs_is_file(path);
}


// as_windows() needed for system calls with MSVC

// force Windows file seperator
char* as_windows(const char* path) {
  char s = '/';
  char* buf = (char*) malloc(strlen(path)+1);  // +1 for null terminator
  strcpy(buf, path);

  char *p = strchr(buf, s);
  while (p) {
    *p = '\\';
    p = strchr(p+1, s);
  }
  return buf;
}


// --- system calls for mkdir and copy_file
int copy_file(const char* source, const char* destination, bool overwrite) {

if(source == NULL || strlen(source) == 0) {
  fprintf(stderr,"ERROR:filesystem:copy_file: source path %s must not be empty\n", source);
  return 1;
}
if(destination == NULL || strlen(destination) == 0) {
  fprintf(stderr, "ERROR:filesystem:copy_file: destination path %s must not be empty\n", destination);
  return 1;
}

  if(overwrite){
    if(fs_is_file(destination)){
      if(!fs_remove(destination)){
        fprintf(stderr, "ERROR:filesystem:copy_file: could not remove existing file %s\n", destination);
      }
    }
  }

  #ifdef _WIN32
  if(CopyFile(source, destination, true))
    return 0;
  return 1;
  #else
// from: https://wiki.sei.cmu.edu/confluence/pages/viewpage.action?pageId=87152177

  char* s = (char*) malloc(strlen(source) + 1);
  char* d = (char*) malloc(strlen(destination) + 1);
  strcpy(s, source);
  strcpy(d, destination);

  char *const args[4] = {"cp", s, d, NULL};

  int ret = execvp("cp", args);
  free(s);
  free(d);

  if(ret != -1)
    return 0;

  return ret;
  #endif
}

int create_directories(const char* path) {
  // Windows: note that SHCreateDirectory is deprecated, so use a system call like Unix

  if(path == NULL || strlen(path) == 0) {
    fprintf(stderr,"ERROR:filesystem:mkdir: path %s must not be empty\n", path);
    return 1;
  }

  if(fs_is_dir(path))
    return 0;

  char* p = (char*) malloc(strlen(path) + 1);

  #ifdef _MSC_VER

  p = as_windows(path);

  STARTUPINFO si = { 0 };
  PROCESS_INFORMATION pi;
  si.cb = sizeof(si);

  char* cmd = (char*) malloc(strlen(p) + 1 + 13);
  strcpy(cmd, "cmd /c mkdir ");
  strcat(cmd, p);

  if(TRACE) printf("TRACE:mkdir %s\n", cmd);
  if (!CreateProcess(NULL, cmd, NULL, NULL, FALSE,
                     0, 0, 0, &si, &pi)) {
    free(p);
    return -1;
  }

  if(TRACE) printf("TRACE:mkdir waiting to complete %s\n", cmd);
  // Wait until child process exits.
  WaitForSingleObject( pi.hProcess, 2000 );
  free(p);
  CloseHandle(pi.hThread);
  CloseHandle(pi.hProcess);
  if(TRACE) printf("TRACE:mkdir completed %s\n", cmd);

  return 0;

  #else
// from: https://wiki.sei.cmu.edu/confluence/pages/viewpage.action?pageId=87152177

  #ifdef _WIN32
  p = as_windows(path);
  char *const args[5] = {"cmd", "/c", "mkdir", p, NULL};
  int ret = execvp("cmd", args);
  #else
  strcpy(p, path);
  char *const args[4] = {"mkdir", "-p", p, NULL};
  int ret = execvp("mkdir", args);
  #endif

  free(p);

  if(ret != -1)
    return 0;

  return ret;
  #endif
}
