#ifndef FILESYSTEM_H
#define FILESYSTEM_H


#ifdef __cplusplus
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

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif
#define MAXP min(PMAX, PATH_LIMIT)
// end maximum path length


extern size_t get_maxp();

extern bool is_macos();
extern bool is_linux();
extern bool is_unix();
extern bool is_windows();

extern size_t fs_filesep(char*);

extern size_t fs_normal(const char*, char*, size_t);

extern size_t join(const char*, const char*, char*, size_t);

extern size_t fs_file_name(const char*, char*, size_t);
extern size_t fs_stem(const char*, char*, size_t);
extern size_t parent(const char*, char*, size_t);
extern size_t suffix(const char*, char*, size_t);
extern size_t root(const char*, char*, size_t);

extern size_t with_suffix(const char*, const char*, char*, size_t);

extern bool is_symlink(const char*);
extern int create_symlink(const char*, const char*);

extern int create_directories(const char*);
extern bool exists(const char*);
extern bool is_absolute(const char*);
extern bool is_dir(const char*);
extern bool is_file(const char*);
extern bool is_exe(const char*);

extern bool chmod_exe(const char*);
extern bool chmod_no_exe(const char*);

extern bool fs_remove(const char*);
extern size_t canonical(const char*, bool, char*, size_t);
extern bool equivalent(const char*, const char*);
extern int copy_file(const char*, const char*, bool);
extern size_t relative_to(const char*, const char*, char*, size_t);
extern bool touch(const char*);

extern size_t get_cwd(char*, size_t);
extern size_t get_homedir(char*, size_t);
extern size_t get_tempdir(char*, size_t);

extern size_t expanduser(const char*, char*, size_t);

extern uintmax_t file_size(const char*);

extern size_t fs_exe_path(char* path, size_t);
extern size_t fs_lib_path(char* path, size_t);

#ifdef __cplusplus
}
#endif

#endif
