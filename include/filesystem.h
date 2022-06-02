#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#endif

extern bool is_macos();
extern bool is_linux();
extern bool is_unix();
extern bool is_windows();

extern size_t as_posix(char*);
extern bool sys_posix();
extern size_t filesep(char*);
extern bool match(const char*, const char*);

extern size_t file_name(const char*, char*);
extern size_t stem(const char*, char*);
extern size_t parent(const char*, char*);
extern size_t suffix(const char*, char*);
extern size_t root(const char*, char*);

extern size_t with_suffix(const char*, const char*, char*);
extern size_t normal(const char*, char*);

extern bool is_symlink(const char*);
extern int create_symlink(const char*, const char*);

extern bool create_directories(const char*);
extern bool exists(const char*);
extern bool is_absolute(const char*);
extern bool is_dir(const char*);
extern bool is_file(const char*);
extern bool is_exe(const char*);

extern bool chmod_exe(const char*);
extern bool chmod_no_exe(const char*);

extern bool fs_remove(const char*);
extern size_t canonical(char*, bool);
extern bool equivalent(const char*, const char*);
extern bool copy_file(const char*, const char*, bool);
extern size_t relative_to(const char*, const char*, char*);
extern bool touch(const char*);

extern size_t get_cwd(char*);
extern size_t get_homedir(char*);
extern size_t get_tempdir(char*);

extern size_t expanduser(const char*, char*);

extern uintmax_t file_size(const char*);

#ifdef __cplusplus
}
#endif

#endif
