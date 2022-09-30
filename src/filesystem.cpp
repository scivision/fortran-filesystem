// functions from C++ filesystem

#include <iostream>
#include <algorithm>
#include <cstring>
#include <string>
#include <fstream>
#include <regex>

#if __has_include(<filesystem>)
#include <filesystem>
namespace fs = std::filesystem;
#else
#error "No C++ filesystem support"
#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include "ffilesystem.h"

#define TRACE 0


size_t path2str(const fs::path p, char* result, size_t buffer_size){

  auto s = p.generic_string();
  std::strncpy(result, s.c_str(), buffer_size);
  size_t L = std::strlen(result);
  result[L] = '\0';

  return L;
}


size_t fs_filesep(char* sep) {

  fs::path p("/");

  std::strncpy(sep, p.make_preferred().string().c_str(), 1);
  sep[1] = '\0';

  return 1;
}


size_t fs_normal(const char* path, char* result, size_t buffer_size) {
  // normalize path
  fs::path p(path);

  return path2str(p.lexically_normal(), result, buffer_size);
}


size_t fs_file_name(const char* path, char* result, size_t buffer_size) {

  fs::path p(path);

  return path2str(p.filename(), result, buffer_size);
}


size_t fs_stem(const char* path, char* result, size_t buffer_size) {

  fs::path p(path);

  return path2str(p.filename().stem(), result, buffer_size);
}


size_t fs_join(const char* path, const char* other, char* result, size_t buffer_size) {

  fs::path p1(path);
  fs::path p2(other);

  return path2str(p1 / p2, result, buffer_size);
}


size_t fs_parent(const char* path, char* result, size_t buffer_size) {

  fs::path p(path);

  p = p.lexically_normal();

  if(p.has_parent_path())
    return path2str(p.parent_path(), result, buffer_size);

  std::strncpy(result, ".", buffer_size);
  size_t L = std::strlen(result);
  result[L] = '\0';

  return L;
}


size_t fs_suffix(const char* path, char* result, size_t buffer_size) {

  fs::path p(path);

  return path2str(p.filename().extension(), result, buffer_size);
}


size_t fs_with_suffix(const char* path, const char* new_suffix, char* result, size_t buffer_size) {

  if(path == nullptr)
    return 0;

  fs::path p(path);

  return path2str(p.replace_extension(new_suffix), result, buffer_size);
}


bool fs_is_symlink(const char* path) {

if(!fs_exists(path))
  return false;

#ifdef __MINGW32__
// c++ filesystem is_symlink doesn't work on MinGW GCC, but this C method does work
  return GetFileAttributes(path) & FILE_ATTRIBUTE_REPARSE_POINT;
#endif

  std::error_code ec;

  auto e = fs::is_symlink(path, ec);
  if(ec) {
    std::cerr << "ERROR:filesystem:is_symlink: " << ec.message() << std::endl;
    return false;
  }

  return e;
}

int fs_create_symlink(const char* target, const char* link) {

  if(target==nullptr || strlen(target) == 0) {
    std::cerr << "ERROR:filesystem:create_symlink: target path must not be empty" << std::endl;
    return 1;
  }
  if(link==nullptr || strlen(link) == 0) {
    std::cerr << "ERROR:filesystem:create_symlink: link path must not be empty" << std::endl;
    return 1;
  }

#ifdef _WIN32
  // C++ filesystem doesn't work for create_symlink, but this C method does work
  if(fs_is_dir(target)) {
    return !(CreateSymbolicLink(link, target,
      SYMBOLIC_LINK_FLAG_DIRECTORY | SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE) != 0);
  }
  else {
    return !(CreateSymbolicLink(link, target,
      SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE) != 0);
  }
#endif

  std::error_code ec;

  if (fs_is_dir(target)) {
    fs::create_directory_symlink(target, link, ec);
  }
  else {
    fs::create_symlink(target, link, ec);
  }
  if(ec) {
    std::cerr << "ERROR:filesystem:create_symlink: " << ec.message() << " " << ec.value() << std::endl;
    return ec.value();
  }

  return 0;
}

int create_directories(const char* path) {

  if(strlen(path) == 0) {
    std::cerr << "ERROR:filesystem:mkdir:create_directories: cannot mkdir empty directory name" << std::endl;
    return 1;
  }

  std::error_code ec;

  auto s = fs::status(path, ec);
  if(s.type() != fs::file_type::not_found){
    if(ec) {
      std::cerr << "ERROR:filesystem:create_directories:status: " << ec.message() << std::endl;
      return ec.value();
    }
  }

  if(fs::exists(s)) {
    if(fs_is_dir(path)) return 0;

    std::cerr << "ERROR:filesystem:mkdir:create_directories: " << path << " already exists but is not a directory" << std::endl;
    return 1;
  }

  auto ok = fs::create_directories(path, ec);
  if(ec) {
    std::cerr << "ERROR:filesystem:create_directories: " << ec.message() << std::endl;
    return ec.value();
  }

  if( !ok ) {
    // old MacOS return != 0 even if directory was created
    if(fs_is_dir(path)) {
      return 0;
    }
    else
    {
      std::cerr << "ERROR:filesystem:mkdir:create_directories: " << path << " could not be created" << std::endl;
      return 1;
    }
  }

  return 0;
}


size_t fs_root(const char* path, char* result, size_t buffer_size) {
  fs::path p(path);

#ifdef _WIN32
  return path2str(p.root_name(), result, buffer_size);
#else
  return path2str(p.root_path(), result, buffer_size);
#endif

}


bool fs_exists(const char* path) {
  std::error_code ec;

  auto e = fs::exists(path, ec);

  if(ec) {
    std::cerr << "ERROR:filesystem:exists: " << ec.message() << std::endl;
    return false;
  }

  return e;
}


bool fs_is_absolute(const char* path) {
  fs::path p(path);
  return p.is_absolute();
}


bool fs_is_dir(const char* path) {

  if(std::strlen(path) == 0)
    return false;

#ifdef _WIN32
  fs::path p(path);
  if (p.root_name() == p)
    return true;
#endif

  if (!fs_exists(path))
    return false;

  return fs::is_directory(path);
}


bool fs_is_exe(const char* path) {

  if (!fs_is_file(path))
    return false;

  auto s = fs::status(path);

  auto i = s.permissions() & (fs::perms::owner_exec | fs::perms::group_exec | fs::perms::others_exec);
  auto isexe = i != fs::perms::none;

  if(TRACE) std::cout << "TRACE:is_exe: " << path << " " << isexe << std::endl;

  return isexe;
}


bool fs_is_file(const char* path) {
  std::error_code ec;

  if (!fs_exists(path))
    return false;

  return fs::is_regular_file(path);
}


bool fs_remove(const char* path) {
  std::error_code ec;

  auto e = fs::remove(path, ec);

  if(ec) {
    std::cerr << "ERROR:filesystem:remove: " << ec.message() << std::endl;
    return false;
  }

  return e;
}

size_t canonical(const char* path, bool strict, char* result, size_t buffer_size) {
  // also expands ~

  if( path == nullptr || strlen(path) == 0 )
    return 0;

  char* ex = new char[buffer_size];
  fs_expanduser(path, ex, buffer_size);

  if(TRACE) std::cout << "TRACE:canonical: input: " << path << " expanded: " << ex << std::endl;

  fs::path p;
  std::error_code ec;

  if(strict){
    p = fs::canonical(ex, ec);
  }
  else {
    p = fs::weakly_canonical(ex, ec);
  }
  delete[] ex;

  if(TRACE) std::cout << "TRACE:canonical: " << p << std::endl;

  if(ec) {
    std::cerr << "ERROR:filesystem:canonical: " << ec.message() << std::endl;
    return 0;
  }

  return path2str(p, result, buffer_size);
}


bool equivalent(const char* path1, const char* path2) {
  // check existance to avoid error if not exist

  if (! (fs_exists(path1) && fs_exists(path2)) )
    return false;

  std::error_code ec;

  auto e = fs::equivalent(path1, path2, ec);

  if(ec) {
    std::cerr << "ERROR:filesystem:equivalent: " << ec.message() << std::endl;
    return false;
  }

  return e;
}


int copy_file(const char* source, const char* destination, bool overwrite) {

  if(strlen(source) == 0) {
    std::cerr << "filesystem:copy_file: source path must not be empty" << std::endl;
    return 1;
  }
  if(strlen(destination) == 0) {
    std::cerr << "filesystem:copy_file: destination path must not be empty" << std::endl;
    return 1;
  }

  auto opt = fs::copy_options::none;

  if (overwrite) {
// WORKAROUND: Windows MinGW GCC 11, Intel oneAPI Linux: bug with overwrite_existing failing on overwrite
    if(fs_exists(destination)){
      if (!fs_remove(destination))
        return 1;
    }

    opt |= fs::copy_options::overwrite_existing;
  }

  std::error_code ec;
  auto ok = fs::copy_file(source, destination, opt, ec);

  if(ec) {
    std::cerr << "ERROR:filesystem:copy_file: " << ec.message() << std::endl;
    return ec.value();
  }

  if( !ok ) {
    if(fs_is_file(destination)) {
      return 0;
    }
    else
    {
      std::cerr << "ERROR:filesystem:copy_file: " << destination << " could not be created" << std::endl;
      return 1;
    }
  }

  return 0;
}


size_t relative_to(const char* to, const char* from, char* result, size_t buffer_size) {

  // undefined case, avoid bugs with MacOS
  if( to == nullptr || (strlen(to) == 0) || from == nullptr || (strlen(from) == 0) )
    return 0;

  fs::path tp(to);
  fs::path fp(from);

  // cannot be relative, avoid bugs with MacOS
  if(tp.is_absolute() != fp.is_absolute())
    return 0;

  std::error_code ec;

  auto r = fs::relative(tp, fp, ec);

  if(ec) {
    std::cerr << "ERROR:filesystem:relative_to: " << ec.message() << std::endl;
    return 0;
  }

  return path2str(r, result, buffer_size);
}


bool touch(const char* path) {

  if(path == nullptr || strlen(path) == 0)
    return false;

  std::error_code ec;

  auto s = fs::status(path, ec);
  if(s.type() != fs::file_type::not_found){
    if(ec) {
      std::cerr << "ERROR:filesystem:touch:status: " << ec.message() << std::endl;
      return false;
    }
  }

  if (fs::exists(s) && !fs::is_regular_file(s))
    return false;

  if(!fs::is_regular_file(s)) {
    std::ofstream ost;
    ost.open(path);
    ost.close();
    // ensure user can access file, as default permissions may be mode 600 or such
    fs::permissions(path, fs::perms::owner_read | fs::perms::owner_write, fs::perm_options::add, ec);
  }
  if(ec) {
    std::cerr << "filesystem:touch:permissions: " << ec.message() << std::endl;
    return false;
  }

  if (!fs_is_file(path))
    return false;

  fs::last_write_time(path, fs::file_time_type::clock::now(), ec);
  if(ec) {
    std::cerr << "filesystem:touch:last_write_time: " << path << " was created, but modtime was not updated: " << ec.message() << std::endl;
    return false;
  }

  return true;

}


size_t fs_get_tempdir(char* result, size_t buffer_size) {

  std::error_code ec;

  auto r = fs::temp_directory_path(ec);

  if(ec) {
    std::cerr << "filesystem:get_tempdir: " << ec.message() << std::endl;
    return 0;
  }

  return path2str(r, result, buffer_size);
}


uintmax_t fs_file_size(const char* path) {
  // need to check is_regular_file for MSVC/Intel Windows

  if (!fs_is_file(path)) {
    std::cerr << "filesystem:file_size: " << path << " is not a regular file" << std::endl;
    return 0;
  }

  std::error_code ec;

  auto fsize = fs::file_size(path, ec);
  if (ec) {
    std::cerr << "ERROR:filesystem:file_size: " << path << " could not get file size: " << ec.message() << std::endl;
    return 0;
  }

  return fsize;
}


size_t fs_get_cwd(char* result, size_t buffer_size) {
  std::error_code ec;

  auto r = fs::current_path(ec);

  if(ec) {
    std::cerr << "ERROR:filesystem:get_cwd: " << ec.message() << std::endl;
    return 0;
  }

  return path2str(r, result, buffer_size);
}


size_t fs_get_homedir(char* result, size_t buffer_size) {

#ifdef _WIN32
  auto k = "USERPROFILE";
#else
  auto k = "HOME";
#endif

  auto r = std::getenv(k);

  if(r == nullptr) {
    std::cerr << "ERROR:filesystem:get_homedir: " << k << " is not defined" << std::endl;
    return 0;
  }

  return fs_normal(r, result, buffer_size);
}


size_t fs_expanduser(const char* path, char* result, size_t buffer_size){

  std::string p(path);

  if(TRACE)  std::cout << "TRACE:expanduser: path: " << p << " length: " << strlen(path) << std::endl;

  if( path == nullptr || strlen(path) == 0 )
    return 0;

  if(p.front() != '~')
    return fs_normal(path, result, buffer_size);

  char* h = new char[buffer_size];
  if (!fs_get_homedir(h, buffer_size)){
    delete[] h;
    return fs_normal(path, result, buffer_size);
  }

  fs::path home(h);
  delete[] h;

  // std::cout << "TRACE:expanduser: path(home) " << home << std::endl;

// drop duplicated separators
  std::regex r("/{2,}");

  std::replace(p.begin(), p.end(), '\\', '/');
  p = std::regex_replace(p, r, "/");

  if(TRACE) std::cout << "TRACE:expanduser: path deduped " << p << std::endl;

  if (p.length() < 3) {
    // ~ alone
    return path2str(home, result, buffer_size);
  }

  return fs_normal((home / p.substr(2)).generic_string().c_str(), result, buffer_size);
}

bool fs_chmod_exe(const char* path) {
  // make path owner executable, if it's a file

  if(!fs_is_file(path)) {
    std::cerr << "filesystem:chmod_exe: " << path << " is not a regular file" << std::endl;
    return false;
  }

  std::error_code ec;

  fs::permissions(path, fs::perms::owner_exec, fs::perm_options::add, ec);

  if(ec) {
    std::cerr << "ERROR:filesystem:chmod_exe: " << path << ": " << ec.message() << std::endl;
    return false;
  }

  return true;

}

bool fs_chmod_no_exe(const char* path) {
  // make path not executable, if it's a file

  if(!fs_is_file(path)) {
    std::cerr << "filesystem:chmod_no_exe: " << path << " is not a regular file" << std::endl;
    return false;
  }

  std::error_code ec;

  fs::permissions(path, fs::perms::owner_exec, fs::perm_options::remove, ec);

  if(ec) {
    std::cerr << "ERROR:filesystem:chmod_no_exe: " << path << ": " << ec.message() << std::endl;
    return false;
  }

  return true;

}
