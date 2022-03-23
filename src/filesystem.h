extern "C" size_t filesep(char*);
extern "C" size_t as_posix(char*);
extern "C" bool is_dir(const char*);
extern "C" size_t get_homedir(char*);
extern "C" size_t expanduser(const char*, char*);

extern "C" bool is_macos();
extern "C" bool is_linux();
extern "C" bool is_unix();
extern "C" bool is_windows();

extern "C" bool create_symlink(const char*, const char*);
extern "C" bool copy_file(const char*, const char*, bool);
