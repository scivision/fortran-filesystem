#ifndef FFILESYSTEM_H
#define FFILESYSTEM_H

#ifndef FS_TRACE
#define FS_TRACE 0
#endif

// maximum path length
#if defined (__APPLE__)
#include <sys/syslimits.h>
#elif !defined (_MSC_VER)
#ifdef __cplusplus
#include <climits>
#else
#include <limits.h>
#endif
#endif
// end maximum path length


#ifdef __cplusplus

#include <cstdint>
#include <cstdlib>
#include <algorithm> // std::min
#include <string>
#include <filesystem>

#ifdef __cpp_lib_filesystem

namespace fs = std::filesystem;


std::string fs_as_posix(std::string_view);
std::string fs_as_windows(std::string_view);
std::string fs_as_cygpath(std::string_view);

std::string fs_normal(std::string_view);
std::string fs_file_name(std::string_view);
std::string fs_stem(std::string_view);
std::string fs_join(std::string_view, std::string_view);
std::string fs_parent(std::string_view);
std::string fs_suffix(std::string_view);
std::string fs_with_suffix(std::string_view, std::string_view);

std::string fs_which(std::string_view);

bool fs_is_symlink(std::string_view);
void fs_create_symlink(std::string_view, std::string_view);
std::string fs_read_symlink(std::string_view);

void fs_create_directories(std::string_view);

std::string fs_root(std::string_view);

bool fs_is_absolute(std::string_view);
bool fs_is_char_device(std::string_view);

bool fs_is_dir(std::string_view);
bool fs_is_exe(std::string_view);
bool fs_is_readable(std::string_view);
bool fs_is_writable(std::string_view);

bool fs_remove(std::string_view);

std::string fs_canonical(std::string_view, bool);
std::string fs_resolve(std::string_view, bool);

bool fs_equivalent(std::string_view, std::string_view);

void fs_copy_file(std::string_view, std::string_view, bool);

std::string fs_relative_to(std::string_view, std::string_view);

void fs_touch(std::string_view);

std::string fs_get_tempdir();

std::string fs_get_cwd();
void fs_set_cwd(std::string_view path);

uintmax_t fs_file_size(std::string_view);
uintmax_t fs_space_available(std::string_view);

bool fs_is_subdir(std::string_view, std::string_view);

void fs_set_permissions(std::string_view, int, int, int);

std::string fs_get_permissions(std::string_view);

bool fs_exists(std::string_view);
std::string fs_expanduser(std::string_view);

std::string fs_get_homedir();

bool fs_is_file(std::string_view);
bool fs_is_reserved(std::string_view);

std::string fs_exe_path();
std::string fs_exe_dir();

std::string fs_lib_path();
std::string fs_lib_dir();

std::string fs_make_absolute(std::string_view, std::string_view);

std::string fs_compiler();

std::string fs_make_tempdir(std::string);

std::string fs_short2long(std::string_view);
std::string fs_long2short(std::string_view);

#endif // __cpp_lib_filesystem

extern "C" {

#else

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#endif


extern bool fs_cpp();
extern long fs_lang();
extern size_t fs_get_max_path();

extern char fs_pathsep();

extern bool fs_is_admin();
extern bool fs_is_bsd();
extern bool fs_is_macos();
extern bool fs_is_linux();
extern bool fs_is_unix();
extern bool fs_is_windows();
extern int fs_is_wsl();
extern bool fs_is_mingw();
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

extern size_t fs_which(const char*, char*, size_t);

extern bool fs_is_symlink(const char*);
extern bool fs_create_symlink(const char*, const char*);
extern size_t fs_read_symlink(const char*, char*, size_t);

extern bool fs_create_directories(const char*);
extern bool fs_exists(const char*);

extern bool fs_is_absolute(const char*);
extern bool fs_is_char_device(const char*);
extern bool fs_is_dir(const char*);
extern bool fs_is_file(const char*);
extern bool fs_is_exe(const char*);
extern bool fs_is_readable(const char*);
extern bool fs_is_writable(const char*);
extern bool fs_is_reserved(const char*);
extern bool fs_is_subdir(const char*, const char*);

extern bool fs_set_permissions(const char*, int, int, int);

extern size_t fs_get_permissions(const char*, char*, size_t);

extern bool fs_remove(const char*);

extern size_t fs_canonical(const char*, bool, char*, size_t);
extern size_t fs_resolve(const char*, bool, char*, size_t);

extern bool fs_equivalent(const char*, const char*);
extern bool fs_copy_file(const char*, const char*, bool);
extern size_t fs_relative_to(const char*, const char*, char*, size_t);
extern bool fs_touch(const char*);

extern size_t fs_get_cwd(char*, size_t);
extern bool fs_set_cwd(const char*);
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

size_t fs_make_tempdir(char*, size_t);

size_t fs_short2long(const char*, char*, size_t);
size_t fs_long2short(const char*, char*, size_t);

#ifdef __cplusplus
}
#endif

#endif
