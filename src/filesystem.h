extern "C" bool is_macos();
extern "C" bool is_linux();
extern "C" bool is_unix();
extern "C" bool is_windows();

extern "C" size_t as_posix(char*);
extern "C" bool sys_posix();
extern "C" size_t filesep(char*);
extern "C" bool match(const char*, const char*);

extern "C" size_t file_name(const char*, char*);
extern "C" size_t stem(const char*, char*);
extern "C" size_t parent(const char*, char*);
extern "C" size_t suffix(const char*, char*);
extern "C" size_t root(const char*, char*);

extern "C" size_t with_suffix(const char*, const char*, char*);
extern "C" size_t normal(const char*, char*);

extern "C" bool is_symlink(const char*);
extern "C" bool create_symlink(const char*, const char*);
extern "C" bool create_directory_symlink(const char*, const char*);
extern "C" bool create_directories(const char*);
extern "C" bool exists(const char*);
extern "C" bool is_absolute(const char*);
extern "C" bool is_dir(const char*);
extern "C" bool is_file(const char*);
extern "C" bool is_exe(const char*);

extern "C" bool chmod_exe(const char*);
extern "C" bool chmod_no_exe(const char*);

extern "C" bool fs_remove(const char*);
extern "C" size_t canonical(char*, bool);
extern "C" bool equivalent(const char*, const char*);
extern "C" bool copy_file(const char*, const char*, bool);
extern "C" size_t relative_to(const char*, const char*, char*);
extern "C" bool touch(const char*);

extern "C" size_t get_cwd(char*);
extern "C" size_t get_homedir(char*);
extern "C" size_t get_tempdir(char*);

extern "C" size_t expanduser(const char*, char*);

extern "C" uintmax_t file_size(const char*);

extern "C" bool create_symlink(const char*, const char*);
extern "C" bool copy_file(const char*, const char*, bool);
