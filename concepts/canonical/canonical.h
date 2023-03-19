#ifdef __cplusplus

#include <string>

#ifdef _WIN32
std::string fs_win32_read_symlink(std::string);
#endif

extern "C" {
#endif

size_t fs_realpath(const char*, char*, size_t);

#ifdef _WIN32
size_t fs_win32_read_symlink(const char*, char*, size_t);
#endif

#ifdef __cplusplus
}
#endif
