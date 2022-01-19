// functions from C++17 filesystem

#include <iostream>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

extern "C" size_t filesep(char*);
extern "C" size_t as_posix(char*);
extern "C" bool is_dir(const char*);


extern "C" size_t as_posix(char* path){
    std::string s(path);

    std::replace(s.begin(), s.end(), '\\', '/');
    strcpy(path, s.c_str());

    return strlen(path);
}


extern "C" bool sys_posix() {
  char sep[2];

  filesep(sep);
  return strcmp(sep, "/") == 0;
}

extern "C" size_t filesep(char* sep) {
  fs::path p("/");

  std::strcpy(sep, p.make_preferred().string().c_str());
  return strlen(sep);
}


extern "C" size_t file_name(const char* path, char* filename) {
  fs::path p(path);

  std::strcpy(filename, p.filename().string().c_str());
  return strlen(filename);
}


extern "C" size_t stem(const char* path, char* fstem) {
  fs::path p(path);

  std::strcpy(fstem, p.stem().string().c_str());
  return strlen(fstem);
}


extern "C" size_t parent(const char* path, char* fparent) {
  fs::path p(path);

  if(p.has_parent_path()){
    std::strcpy(fparent, p.parent_path().string().c_str());
  }
  else{
    std::strcpy(fparent, ".");
  }

  return strlen(fparent);
}


extern "C" size_t suffix(const char* path, char* fsuffix) {
  fs::path p(path);

  std::strcpy(fsuffix, p.extension().string().c_str());
  return strlen(fsuffix);
}


extern "C" size_t with_suffix(const char* path, const char* new_suffix, char* swapped) {
  fs::path p(path);

  std::strcpy(swapped, p.replace_extension(new_suffix).string().c_str());
  return strlen(swapped);
}



extern "C" size_t normal(const char* path, char* normalized) {
  fs::path p(path);

  std::strcpy(normalized, p.lexically_normal().string().c_str());

  return as_posix(normalized);
}


extern "C" bool is_symlink(const char* path) {
  return fs::is_symlink(path);
}

extern "C" void create_symlink(const char* target, const char* link) {

  if(strlen(target) == 0) {
    std::cerr << "pathlib:create_symlink: target path must not be empty" << std::endl;
    exit(EXIT_FAILURE);
  }
  if(strlen(link) == 0) {
    std::cerr << "pathlib:create_symlink: link path must not be empty" << std::endl;
    exit(EXIT_FAILURE);
  }

  if (is_dir(target)) {
    fs::create_directory_symlink(target, link);
  }
  else {
    fs::create_symlink(target, link);
  }
}

extern "C" void create_directory_symlink(const char* target, const char* link) {
  fs::create_directory_symlink(target, link);
}

extern "C" bool create_directories(const char* path) {

  if(strlen(path) == 0) {
    std::cerr << "pathlib:mkdir:create_directories: cannot mkdir empty directory name" << std::endl;
    exit(EXIT_FAILURE);
  }

  auto s = fs::status(path);

  if(fs::exists(s)) {
    if(is_dir(path)) return true;

    std::cerr << "pathlib:mkdir:create_directories: " << path << " already exists but is not a directory" << std::endl;
    exit(EXIT_FAILURE);
  }

  auto ok = fs::create_directories(path);

  if( !ok ) {
    // old MacOS return false even if directory was created
    if(is_dir(path)) {
      return true;
    }
    else
    {
      std::cerr << "pathlib:mkdir:create_directories: " << path << " could not be created" << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  return ok;
}


extern "C" size_t root(const char* path, char* result) {
  fs::path p(path);
  fs::path r;

#ifdef _WIN32
  r = p.root_name();
#else
  r = p.root_path();
#endif

  std::strcpy(result, r.string().c_str());

  return strlen(result);
}

extern "C" bool exists(const char* path) {
  return fs::exists(path);
}

extern "C" bool is_absolute(const char* path) {
  fs::path p(path);
  return p.is_absolute();
}

extern "C" bool is_file(const char* path) {
  return fs::is_regular_file(path);
}

extern "C" bool is_dir(const char* path) {
  if(std::strlen(path) == 0) return false;

  fs::path p(path);

#ifdef _WIN32
  if (p.root_name() == p) return true;
#endif

  return fs::is_directory(p);
}

extern "C" bool fs_remove(const char* path) {
  return fs::remove(path);
}

extern "C" size_t canonical(char* path, bool strict){
  // does NOT expand tilde ~

  if( (strlen(path) == 0) ) {
    path = NULL;
    return 0;
  }

  fs::path p;

  if(strict){
    p = fs::canonical(path);
  }
  else {
    p = fs::weakly_canonical(path);
  }

  std::strcpy(path, p.string().c_str());
  return as_posix(path);
}


extern "C" bool equivalent(const char* path1, const char* path2) {
  // check existance to avoid error if not exist
  fs::path p1(path1);
  fs::path p2(path2);

  if (! (fs::exists(p1) & fs::exists(p2)) ) return false;

  return fs::equivalent(p1, p2);
}

extern "C" bool copy_file(const char* source, const char* destination, bool overwrite) {

  if(strlen(source) == 0) {
    std::cerr << "pathlib:copy_file: source path must not be empty" << std::endl;
    exit(EXIT_FAILURE);
  }
  if(strlen(destination) == 0) {
    std::cerr << "pathlib:copy_file: destination path must not be empty" << std::endl;
    exit(EXIT_FAILURE);
  }

  fs::path d(destination);

  auto opt = fs::copy_options::none;

  if (overwrite) {

// WORKAROUND: Windows MinGW GCC 11, Intel oneAPI Linux: bug with overwrite_existing failing on overwrite
  if(fs::exists(d)) fs::remove(d);

  opt |= fs::copy_options::overwrite_existing;
  }

  return fs::copy_file(source, d, opt);
}


extern "C" size_t relative_to(const char* a, const char* b, char* result) {

  // library bug handling
  if( (strlen(a) == 0) | (strlen(b) == 0) ) {
    // undefined case, avoid bugs with MacOS
    result = NULL;
    return 0;
  }

  fs::path a1(a);
  fs::path b1(b);

  if(a1.is_absolute() != b1.is_absolute()) {
    // cannot be relative, avoid bugs with MacOS
    result = NULL;
    return 0;
  }

  auto r = fs::relative(a1, b1);

  std::strcpy(result, r.string().c_str());
  return as_posix(result);
}


extern "C" bool touch(const char* path) {

  if(strlen(path) == 0) {
    std::cerr << "pathlib:touch: cannot touch empty file name" << std::endl;
    exit(EXIT_FAILURE);
  }

  fs::path p(path);

  auto s = fs::status(p);

  if (fs::exists(s) & !fs::is_regular_file(s)) return false;

  if(!fs::is_regular_file(s)) {
    std::ofstream ost;
    ost.open(p);
    ost.close();
  }

  if (!fs::is_regular_file(p)) return false;
  // here p because we want to check the new file

  fs::last_write_time(p, std::filesystem::file_time_type::clock::now());

  return true;

}


extern "C" size_t get_tempdir(char* path) {
  std::strcpy(path, fs::temp_directory_path().string().c_str());
  return as_posix(path);
}


extern "C" uintmax_t file_size(const char* path) {
  fs::path p(path);

  if (!fs::is_regular_file(p)) return -1;

  return fs::file_size(p);
}


extern "C" size_t get_cwd(char* path) {
  std::strcpy(path, fs::current_path().string().c_str());
  return as_posix(path);
}


extern "C" bool is_exe(const char* path) {
  fs::path p(path);

  auto s = fs::status(p);

  if (!fs::is_regular_file(s)) return false;

  auto i = s.permissions() & (fs::perms::owner_exec | fs::perms::group_exec | fs::perms::others_exec);
  return i != fs::perms::none;
}
