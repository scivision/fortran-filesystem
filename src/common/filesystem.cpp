// functions from C++ filesystem

// NOTE: this segfaults: std::filesystem::path p(nullptr);

#include <iostream>
#include <algorithm>
#include <cstring>
#include <string>
#include <fstream>
#include <regex>
#include <set>

#if __has_include(<filesystem>)
#include <filesystem>
namespace fs = std::filesystem;
#else
#error "No C++ filesystem support"
#endif

#include "ffilesystem.h"

#ifdef __MINGW32__
#include "windows.c"
#endif


bool fs_cpp()
{
// tell if fs core is C or C++
  return true;
}

static size_t fs_path2str(const fs::path p, char* result, size_t buffer_size)
{
  auto s = p.generic_string();

  if(TRACE)
    std::cout << "TRACE:fs_path2str: " << s << " " << s.length() << " " << buffer_size << std::endl;

  if(s.length() >= buffer_size){
    result = nullptr;
    std::cerr << "ERROR:ffilesystem: output buffer too small for path: " << s << std::endl;
    return 0;
  }
  std::strncpy(result, s.c_str(), buffer_size);
  size_t L = std::strlen(result);
  result[L] = '\0';

  return L;
}


size_t fs_normal(const char* path, char* result, size_t buffer_size)
{
  // normalize path
  if(!path){
    result = nullptr;
    return 0;
  }

  fs::path p(path);

  return fs_path2str(p.lexically_normal(), result, buffer_size);
}


size_t fs_file_name(const char* path, char* result, size_t buffer_size)
{
  if(!path){
    result = nullptr;
    return 0;
  }

  fs::path p(path);

  return fs_path2str(p.filename(), result, buffer_size);
}


size_t fs_stem(const char* path, char* result, size_t buffer_size)
{
  if(!path){
    result = nullptr;
    return 0;
  }

  fs::path p(path);

  return fs_path2str(p.filename().stem(), result, buffer_size);
}


size_t fs_join(const char* path, const char* other, char* result, size_t buffer_size)
{
  if(path == nullptr || other == nullptr){
    result = nullptr;
    return 0;
  }

  size_t L1 = std::strlen(path);
  size_t L2 = std::strlen(other);

  if (L1 == 0 && L2 == 0){
    result[0] = '\0';
    return 0;
  }

  fs::path p1(path);
  fs::path p2(other);

  if (TRACE)
    std::cout << "TRACE:fs_join: " << path << " + " << other << std::endl;

  if(L1 == 0)
    return fs_path2str(p2, result, buffer_size);

  if(L2 == 0)
    return fs_path2str(p1, result, buffer_size);

  return fs_path2str(p1 / p2, result, buffer_size);
}


size_t fs_parent(const char* path, char* result, size_t buffer_size)
{
  if(!path){
    result = nullptr;
    return 0;
  }

  fs::path p(path);

  return fs_path2str(p.lexically_normal().parent_path(), result, buffer_size);
}


size_t fs_suffix(const char* path, char* result, size_t buffer_size)
{
  if(!path){
    result = nullptr;
    return 0;
  }

  fs::path p(path);

  return fs_path2str(p.filename().extension(), result, buffer_size);
}


size_t fs_with_suffix(const char* path, const char* new_suffix,
                      char* result, size_t buffer_size)
{
  if(!path){
    result = nullptr;
    return 0;
  }

  fs::path p(path);

  return fs_path2str(p.replace_extension(new_suffix), result, buffer_size);
}


bool fs_is_symlink(const char* path)
{
  if(!path)
    return 0;

#ifdef __MINGW32__
// c++ filesystem is_symlink doesn't work on MinGW GCC, but this C method does work
  return fs_win32_is_symlink(path);
#endif

  std::error_code ec;

  auto e = fs::is_symlink(path, ec);
  if(ec) {
    std::cerr << "ERROR:filesystem:is_symlink: " << ec.message() << std::endl;
    return false;
  }

  return e;
}

int fs_create_symlink(const char* target, const char* link)
{
  if(!fs_exists(target)) {
    std::cerr << "ERROR:filesystem:create_symlink: target path does not exist" << std::endl;
    return 1;
  }
  if(!link || std::strlen(link) == 0) {
    std::cerr << "ERROR:filesystem:create_symlink: link path must not be empty" << std::endl;
    return 1;
  }

#ifdef __MINGW32__
  // C++ filesystem doesn't work for create_symlink with MinGW, but this C method does work
  return fs_win32_create_symlink(target, link);
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

int fs_create_directories(const char* path)
{
  if(!path || std::strlen(path) == 0) {
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


size_t fs_root(const char* path, char* result, size_t buffer_size)
{
  if(!path){
    result = nullptr;
    return 0;
  }

  fs::path p(path);

  return fs_path2str(p.root_path(), result, buffer_size);
}


bool fs_exists(const char* path)
{
  if(!path)
    return false;

  if(fs_is_reserved(path))
    return true;

  std::error_code ec;

  auto e = fs::exists(path, ec);

  if(ec) {
    std::cerr << "ERROR:ffilesystem:exists: " << ec.message() << std::endl;
    return false;
  }

  return e;
}


bool fs_is_absolute(const char* path)
{
  if(!path)
    return 0;

  fs::path p(path);
  return p.is_absolute();
}


bool fs_is_dir(const char* path)
{
  if(!path || std::strlen(path) == 0)
    return false;

  fs::path p(path);

#ifdef _WIN32
  if (p.root_name() == p)
    return true;
#endif

  std::error_code ec;
  auto e = fs::is_directory(p, ec);
  if(ec) {
    std::cerr << "ERROR:filesystem:is_dir: " << ec.message() << " " << p << std::endl;
    return false;
  }

  return e;

}


bool fs_is_exe(const char* path)
{
  if (!fs_is_file(path))
    return false;

  auto s = fs::status(path);

  auto i = s.permissions() & (fs::perms::owner_exec | fs::perms::group_exec | fs::perms::others_exec);
  auto isexe = i != fs::perms::none;

  if(TRACE) std::cout << "TRACE:is_exe: " << path << " " << isexe << std::endl;

  return isexe;
}


bool fs_is_file(const char* path)
{
  if(!path)
    return false;

  if(fs_is_reserved(path))
    return false;

  std::error_code ec;

  auto e = fs::is_regular_file(path, ec);
  if(ec) {
    std::cerr << "ERROR:filesystem:is_file: " << ec.message() << std::endl;
    return false;
  }
  return e;
}


bool fs_is_reserved(const char* path)
// https://learn.microsoft.com/en-gb/windows/win32/fileio/naming-a-file#naming-conventions
{

#ifndef _WIN32
  return false;
#endif

  if(!path)
    return false;

  std::set<std::string> reserved {
      "CON", "PRN", "AUX", "NUL",
      "COM0", "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9",
      "LPT0", "LPT1", "LPT2", "LPT3", "LPT4", "LPT5", "LPT6", "LPT7", "LPT8", "LPT9"};

  auto s = std::string(path);
  std::transform(s.begin(), s.end(), s.begin(), ::toupper);

#if __cplusplus >= 202002L
  return reserved.contains(s);
#else
  return reserved.find(s) != reserved.end();
#endif

}


bool fs_remove(const char* path)
{
  if(!path)
    return false;

  std::error_code ec;

  auto e = fs::remove(path, ec);

  if(ec) {
    std::cerr << "ERROR:filesystem:remove: " << ec.message() << std::endl;
    return false;
  }

  return e;
}

size_t fs_canonical(const char* path, bool strict, char* result, size_t buffer_size)
{
  // also expands ~

  fs::path p;
  std::error_code ec;
  char* ex;

  if ( !path || std::strlen(path) == 0 ) goto retnull;

  if ( fs_is_reserved(path) ) {
    std::strncpy(result, path, buffer_size);
    size_t L = std::strlen(result);
    result[L] = '\0';
    return L;
  }

  ex = new char[buffer_size];
  fs_expanduser(path, ex, buffer_size);

  if(TRACE) std::cout << "TRACE:canonical: input: " << path << " expanded: " << ex << std::endl;

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
    goto retnull;
  }

  return fs_path2str(p, result, buffer_size);

retnull:
  result = nullptr;
  return 0;
}


bool fs_equivalent(const char* path1, const char* path2)
{
  // both paths must exist, or they are not equivalent -- return false
  // to behave like filesystem.c, canonicalize first

  char* buf1 = new char[MAXP];
  char* buf2 = new char[MAXP];

  if(!fs_canonical(path1, true, buf1, MAXP) || !fs_canonical(path2, true, buf2, MAXP)) {
    delete[] buf1;
    delete[] buf2;
    return false;
  }

  std::error_code ec;
  auto e = fs::equivalent(buf1, buf2, ec);
  delete [] buf1;
  delete [] buf2;

  if(ec) {
    std::cerr << "ERROR:filesystem:equivalent: " << ec.message() << std::endl;
    return false;
  }

  return e;
}


int fs_copy_file(const char* source, const char* destination, bool overwrite)
{
  if(!source || std::strlen(source) == 0) {
    std::cerr << "filesystem:copy_file: source path must not be empty" << std::endl;
    return 1;
  }
  if(!destination || std::strlen(destination) == 0) {
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


size_t fs_relative_to(const char* to, const char* from, char* result, size_t buffer_size)
{
  fs::path tp, fp, r;
  std::error_code ec;

  // undefined case, avoid bugs with MacOS
  if( !to || (std::strlen(to) == 0) || !from || (std::strlen(from) == 0) ) goto retnull;

  tp = to;
  fp = from;

  // cannot be relative, avoid bugs with MacOS
  if(tp.is_absolute() != fp.is_absolute()){
    result[0] = '\0';
    return 0;
  }

  // Windows special case for reserved paths
  if(fs_is_reserved(to) || fs_is_reserved(from)){
    if(std::strcmp(to, from) == 0){
      result[0] = '.';
      result[1] = '\0';
      return 1;
    }
    goto retnull;
  }

  r = fs::relative(tp, fp, ec);

  if(ec) {
    std::cerr << "ERROR:filesystem:relative_to: " << ec.message() << std::endl;
    goto retnull;
  }

  return fs_path2str(r, result, buffer_size);

retnull:
  result = nullptr;
  return 0;
}


bool fs_touch(const char* path)
{
  if(!path || std::strlen(path) == 0)
    return false;

  fs::path p(path);
  std::error_code ec;

  auto s = fs::status(p, ec);
  if(s.type() != fs::file_type::not_found){
    if(ec) {
      std::cerr << "ERROR:filesystem:touch:status: " << ec.message() << ": " << p << std::endl;
      return false;
    }
  }

  if (fs::exists(s) && !fs::is_regular_file(s)){
    std::cerr << "ERROR:filesystem:touch: " << p << " exists, but is not a regular file" << std::endl;
    return false;
  }

  if(fs::is_regular_file(s)) {

    if ((s.permissions() & fs::perms::owner_write) == fs::perms::none){
      std::cerr << "ERROR:filesystem:touch: " << p << " is not writable" << std::endl;
      return false;
    }

    fs::last_write_time(p, fs::file_time_type::clock::now(), ec);
    if(ec) {
      std::cerr << "filesystem:touch:last_write_time: " << p << " modtime was not updated: " << ec.message() << std::endl;
      return false;
    }
    return true;
  }

  std::ofstream ost;
  ost.open(p, std::ios_base::out);
  if(!ost.is_open()){
    std::cerr << "ERROR:filesystem:touch:open: " << p << " could not be created" << std::endl;
    return false;
  }
  ost.close();
  // ensure user can access file, as default permissions may be mode 600 or such
  fs::permissions(p, fs::perms::owner_read | fs::perms::owner_write, fs::perm_options::add, ec);

  if(ec) {
    std::cerr << "ERROR:filesystem:touch:permissions: " << ec.message() << std::endl;
    return false;
  }

  return fs::is_regular_file(p);
}


size_t fs_get_tempdir(char* path, size_t buffer_size)
{
  std::error_code ec;

  auto r = fs::temp_directory_path(ec);
  if(ec) {
    std::cerr << "ERROR:filesystem:get_tempdir: " << ec.message() << std::endl;
    path = nullptr;
    return 0;
  }

  return fs_path2str(r, path, buffer_size);
}


uintmax_t fs_file_size(const char* path)
{
  // need to check is_regular_file for MSVC/Intel Windows

  if(!path)
    return 0;

  if (!fs_is_file(path)) {
    std::cerr << "ERROR:filesystem:file_size: " << path << " is not a regular file" << std::endl;
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


size_t fs_get_cwd(char* path, size_t buffer_size)
{
  std::error_code ec;

  auto r = fs::current_path(ec);

  if(ec) {
    std::cerr << "ERROR:filesystem:get_cwd: " << ec.message() << std::endl;
    path = nullptr;
    return 0;
  }

  return fs_path2str(r, path, buffer_size);
}


size_t fs_get_homedir(char* path, size_t buffer_size)
{
#ifdef _WIN32
  auto k = "USERPROFILE";
#else
  auto k = "HOME";
#endif

  auto r = std::getenv(k);

  if(!r) {
    std::cerr << "ERROR:filesystem:get_homedir: " << k << " is not defined" << std::endl;
    path = nullptr;
    return 0;
  }

  return fs_normal(r, path, buffer_size);
}


size_t fs_expanduser(const char* path, char* result, size_t buffer_size)
{
  if(!path){
    result = nullptr;
    return 0;
  }

  if(strlen(path) == 0){
    // string does not handle empty string '\0'
    result[0] = '\0';
    return 0;
  }

  std::string p(path);

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
// NOT .lexical_normal to handle "~/.."
  std::regex r("/{2,}");

  std::replace(p.begin(), p.end(), '\\', '/');
  p = std::regex_replace(p, r, "/");

if(TRACE) std::cout << "TRACE:expanduser: path deduped " << p << std::endl;

  if (p.length() < 3) {
    // ~ alone
    return fs_path2str(home, result, buffer_size);
  }

  return fs_normal((home / p.substr(2)).generic_string().c_str(), result, buffer_size);
}

bool fs_chmod_exe(const char* path)
{
  // make path owner executable, if it's a file
  if(!path)
    return false;

  if(!fs_is_file(path)) {
    std::cerr << "ERROR:ffilesystem:chmod_exe: " << path << " is not a regular file" << std::endl;
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

bool fs_chmod_no_exe(const char* path)
{
  // make path not executable, if it's a file
  if(!path)
    return false;

  if(!fs_is_file(path)) {
    std::cerr << "ERROR:ffilesystem:chmod_no_exe: " << path << " is not a regular file" << std::endl;
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
