#ifndef FILESYSTEM_H
#define FILESYSTEM_H


#define TRACE 0

#ifdef __cplusplus

#include <cstdint>

extern "C" {

#else

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
#define PMAX PATH_MAX
#elif defined (_MSC_VER)
#include <stdlib.h>
#define PMAX _MAX_PATH
#else
#include <limits.h>
#ifdef PATH_MAX
#define PMAX PATH_MAX
#endif
#endif

#if !defined(PMAX)
#if defined (_POSIX_PATH_MAX)
#define PMAX _POSIX_PATH_MAX
#else
#define PMAX 256
#endif
#endif

#ifdef __cplusplus
#define MAXP std::min(PMAX, PATH_LIMIT)
#else
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif
#define MAXP min(PMAX, PATH_LIMIT)
#endif
// end maximum path length

extern bool fs_cpp();
extern size_t fs_get_maxp();

extern bool fs_is_macos();
extern bool fs_is_linux();
extern bool fs_is_unix();
extern bool fs_is_windows();

extern void fs_as_posix(char*);
extern void fs_as_windows(char*);

extern size_t fs_filesep(char*);

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
extern bool fs_is_dir(const char*);
extern bool fs_is_file(const char*);
extern bool fs_is_exe(const char*);

extern bool fs_chmod_exe(const char*);
extern bool fs_chmod_no_exe(const char*);

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

extern size_t fs_exe_path(char* path, size_t);
extern size_t fs_exe_dir(char* path, size_t);
extern size_t fs_lib_path(char* path, size_t);
extern size_t fs_lib_dir(char* path, size_t);

// internal functions
bool _fs_win32_is_symlink(const char*);
bool _fs_win32_create_symlink(const char*, const char*);

#ifdef __cplusplus
}
#endif

#endif
