#ifndef FFILESYSTEM_H
#define FFILESYSTEM_H


#define FS_TRACE 0


#ifdef __cplusplus

#include <cstdint>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <filesystem>

#ifdef __cpp_lib_filesystem

namespace fs = std::filesystem;


std::string fs_as_posix(std::string);
std::string fs_as_windows(std::string);
std::string fs_as_cygpath(std::string);

std::string fs_canonical(std::string, bool);
std::string fs_root(std::string);

bool fs_chmod_exe(std::string, bool);
std::string fs_get_permissions(std::string);

int fs_copy_file(std::string, std::string, bool);
int fs_create_directories(std::string);
int fs_create_symlink(std::string, std::string);
bool fs_equivalent(std::string, std::string);
bool fs_exists(std::string);
std::string fs_expanduser(std::string);
std::string fs_file_name(std::string);
uintmax_t fs_file_size(std::string);
std::string fs_get_cwd();
std::string fs_get_homedir();
std::string fs_get_tempdir();
bool fs_is_absolute(std::string);
bool fs_is_char_device(std::string);
bool fs_is_dir(std::string);
bool fs_is_exe(std::string);
bool fs_is_file(std::string);
bool fs_is_reserved(std::string);
bool fs_is_symlink(std::string);
std::string fs_join(std::string, std::string);
std::string fs_normal(std::string);
std::string fs_parent(std::string);
std::string fs_relative_to(std::string, std::string);
bool fs_remove(std::string);
std::string fs_root(std::string);
uintmax_t fs_space_available(std::string);
std::string fs_stem(std::string);
std::string fs_suffix(std::string);
bool fs_touch(std::string);
std::string fs_with_suffix(std::string, std::string);

std::string fs_exe_path();
std::string fs_exe_dir();

std::string fs_lib_path();
std::string fs_lib_dir();

std::string fs_make_absolute(std::string, std::string);

std::string fs_compiler();

size_t fs_str2char(std::string, char*, size_t);
size_t fs_path2str(const fs::path, char*, size_t);

#endif // __cpp_lib_filesystem

extern "C" {

#else

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#endif

// maximum path length
#define PATH_LIMIT 4096
// absolute maximum, in case a system has ill-defined maximum path length

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#endif

#if defined (__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
#include <sys/syslimits.h>
#elif defined (_MSC_VER)
#else
#ifdef __cplusplus
#include <climits>
#else
#include <limits.h>
#endif
#endif

#ifdef __cplusplus
constexpr size_t fs_max_path() {
  size_t m = 256;
  constexpr size_t pmax = 4096;  // arbitrary absolute maximum
#ifdef PATH_MAX
  m = PATH_MAX;
#elif defined (_MAX_PATH)
  m = _MAX_PATH;
#elif defined (_POSIX_PATH_MAX)
  m = _POSIX_PATH_MAX;
#endif
  return std::min(m, pmax);
}
constexpr size_t FS_MAX_PATH = fs_max_path();
#else
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif
#ifdef PATH_MAX
#define FS_MAX_PATH PATH_MAX
#elif defined (_MAX_PATH)
#define FS_MAX_PATH _MAX_PATH
#elif defined (_POSIX_PATH_MAX)
#define FS_MAX_PATH _POSIX_PATH_MAX
#else
#define FS_MAX_PATH 256
#endif
#endif
// end maximum path length

extern bool fs_cpp();
extern size_t fs_get_max_path();

extern bool fs_is_macos();
extern bool fs_is_linux();
extern bool fs_is_unix();
extern bool fs_is_windows();
extern bool fs_is_wsl();
extern bool fs_is_cygwin();

extern void fs_as_posix(char*);
extern void fs_as_windows(char*);

extern size_t fs_normal(const char*, char*, size_t);

extern size_t fs_join(const char*, const char*, char*, size_t);
extern size_t fs_make_absolute(const char*, const char*, char*, size_t);

extern size_t fs_file_name(const char*, char*, size_t);
extern size_t fs_stem(const char*, char*, size_t);
extern size_t fs_parent(const char*, char*, size_t);
extern size_t fs_suffix(const char*, char*, size_t);
extern size_t fs_root(const char*, char*, size_t);

extern size_t fs_with_suffix(const char*, const char*, char*, size_t);

extern bool fs_is_symlink(const char*);
extern int fs_create_symlink(const char*, const char*);

extern int fs_create_directories(const char*);
extern bool fs_exists(const char*);

extern bool fs_is_absolute(const char*);
extern bool fs_is_char_device(const char*);
extern bool fs_is_dir(const char*);
extern bool fs_is_file(const char*);
extern bool fs_is_exe(const char*);
extern bool fs_is_reserved(const char*);

extern bool fs_chmod_exe(const char*, bool);
extern size_t fs_get_permissions(const char*, char*, size_t);

extern bool fs_remove(const char*);
extern size_t fs_canonical(const char*, bool, char*, size_t);
extern bool fs_equivalent(const char*, const char*);
extern int fs_copy_file(const char*, const char*, bool);
extern size_t fs_relative_to(const char*, const char*, char*, size_t);
extern bool fs_touch(const char*);

extern size_t fs_get_cwd(char*, size_t);
extern size_t fs_get_homedir(char*, size_t);
extern size_t fs_get_tempdir(char*, size_t);

extern size_t fs_expanduser(const char*, char*, size_t);

extern uintmax_t fs_file_size(const char*);
extern uintmax_t fs_space_available(const char*);

extern size_t fs_exe_path(char*, size_t);
extern size_t fs_exe_dir(char*, size_t);
extern size_t fs_lib_path(char*, size_t);
extern size_t fs_lib_dir(char*, size_t);

extern size_t fs_compiler(char*, size_t);

bool fs_win32_is_symlink(const char*);
int fs_win32_create_symlink(const char*, const char*);

#ifdef __cplusplus
}
#endif

#endif
