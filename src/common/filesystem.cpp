// functions from C++ filesystem

// NOTE: this segfaults: std::filesystem::path p(nullptr);

#include <iostream>
#include <algorithm>  // IWYU pragma: keep
#include <cstring>
#include <string>
#include <fstream>  // IWYU pragma: keep
#include <regex>
#include <set>
#include <cstdint>
#include <cstdlib>
#include <system_error>
#include <filesystem>

#include "ffilesystem.h"

// for lib_path, exe_path

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#elif defined(__CYGWIN__)
#include <windows.h>
#elif defined(HAVE_DLADDR)
#include <dlfcn.h>
static void dl_dummy_func() {}
#endif

#ifdef __APPLE__
#include <mach-o/dyld.h>
#elif defined(__OpenBSD__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__DragonFly__)
#include <sys/sysctl.h>
#elif defined(__linux__) || defined(__CYGWIN__)
#include <unistd.h>
#endif
// --- end of lib_path, exe_path

size_t fs_get_max_path(){ return FS_MAX_PATH; };


bool fs_cpp()
{
// tell if fs core is C or C++
  return true;
}

size_t fs_str2char(std::string s, char* result, size_t buffer_size)
{
  if(s.length() >= buffer_size){
    result = nullptr;
    std::cerr << "ERROR:ffilesystem: output buffer too small for string: " << s << "\n";
    return 0;
  }

  std::strcpy(result, s.data());
  return std::strlen(result);
}


size_t fs_path2str(const fs::path p, char* result, size_t buffer_size)
{
  return fs_str2char(p.generic_string(), result, buffer_size);
}

size_t fs_compiler(char* name, size_t buffer_size)
{
  return fs_str2char(fs_compiler(), name, buffer_size);
}

std::string fs_compiler()
{
#ifdef __cpp_lib_format

#if defined(__INTEL_LLVM_COMPILER)
  return std::format("Intel LLVM {} {}", __INTEL_LLVM_COMPILER,  __VERSION__);
#elif defined(__NVCOMPILER_LLVM__)
  return std::format("NVIDIA nvc {}.{}.{}", __NVCOMPILER_MAJOR__, __NVCOMPILER_MINOR__, __NVCOMPILER_PATCHLEVEL__);
#elif defined(__clang__)
  return std::format("Clang {}.{}.{}", __clang_major__, __clang_minor__, __clang_patchlevel__);
#elif defined(__GNUC__)
  return std::format("GNU GCC {}.{}.{}", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#elif defined(_MSC_VER)
  return std::format("MSVC {}", _MSC_FULL_VER);
#else
  return "Unknown compiler--file bug report with Ffilesystem project to add support";
#endif

#else
  return "ERROR:ffilesystem: compiler is not C++20 std::format capable";
#endif
}


void fs_as_posix(char* path)
{
  std::replace(path, path + std::strlen(path), '\\', '/');
}

std::string fs_as_posix(std::string path)
{
  // force posix file seperator
  std::replace(path.begin(), path.end(), '\\', '/');
  return path;
}


void fs_as_windows(char* path)
{
  std::replace(path, path + std::strlen(path), '/', '\\');
}

std::string fs_as_windows(std::string path)
{
  // force windows file seperator
  std::replace(path.begin(), path.end(), '/', '\\');
  return path;
}

std::string fs_as_cygpath(std::string path)
{
  // like command line "cygpath --unix"
#ifdef __CYGWIN__
  std::replace(path.begin(), path.end(), '\\', '/');
  if (path[1] == ':' && std::isalpha(path[0]))
    return "/cygdrive/" + path.substr(0, 1) + path.substr(2);
#endif
  return path;
}


size_t fs_normal(const char* path, char* result, size_t buffer_size)
{
  return fs_str2char(fs_normal(std::string(path)), result, buffer_size);
}

std::string fs_normal(std::string path)
{
  // normalize path
  fs::path p(path);
  return p.lexically_normal().generic_string();
}


size_t fs_file_name(const char* path, char* result, size_t buffer_size)
{
  return fs_str2char(fs_file_name(std::string(path)), result, buffer_size);
}

std::string fs_file_name(std::string path)
{
  fs::path p(path);
  return p.filename().generic_string();
}


size_t fs_stem(const char* path, char* result, size_t buffer_size)
{
  return fs_str2char(fs_stem(std::string(path)), result, buffer_size);
}

std::string fs_stem(std::string path)
{
  fs::path p(path);
  return p.filename().stem().generic_string();
}


size_t fs_join(const char* path, const char* other, char* result, size_t buffer_size)
{
  return fs_str2char(fs_join(std::string(path), std::string(other)), result, buffer_size);
}

std::string fs_join(std::string path, std::string other)
{
  if (other.empty())
    return path;

  // join two paths
  fs::path p1(path);

  if (FS_TRACE) std::cout << "TRACE:fs_join: " << path << " / " << other << "\n";

  return (p1 / other).generic_string();
}


size_t fs_parent(const char* path, char* result, size_t buffer_size)
{
  return fs_str2char(fs_parent(std::string(path)), result, buffer_size);
}

std::string fs_parent(std::string path)
{
  fs::path p(path);
  return p.lexically_normal().parent_path().generic_string();
}


size_t fs_suffix(const char* path, char* result, size_t buffer_size)
{
  return fs_str2char(fs_suffix(std::string(path)), result, buffer_size);
}

std::string fs_suffix(std::string path)
{
  fs::path p(path);

  return p.filename().extension().generic_string();
}


size_t fs_with_suffix(const char* path, const char* new_suffix,
                      char* result, size_t buffer_size)
{
  return fs_str2char(fs_with_suffix(std::string(path), std::string(new_suffix)), result, buffer_size);
}

std::string fs_with_suffix(std::string path, std::string new_suffix)
{
  fs::path p(path);
  return p.replace_extension(new_suffix).generic_string();
}


bool fs_is_symlink(const char* path)
{
  return fs_is_symlink(std::string(path));
}

bool fs_is_symlink(std::string path)
{
  if (path.empty())
    return false;

#ifdef __MINGW32__
// c++ filesystem is_symlink doesn't work on MinGW GCC, but this C method does work
  return fs_win32_is_symlink(path.c_str());
#endif

  std::error_code ec;

  auto e = fs::is_symlink(path, ec);
  if(ec) {
    std::cerr << "ERROR:filesystem:is_symlink: " << ec.message() << "\n";
    return false;
  }

  return e;
}

int fs_create_symlink(const char* target, const char* link)
{
  return fs_create_symlink(std::string(target), std::string(link));
}

int fs_create_symlink(std::string target, std::string link)
{
  if(!fs_exists(target)) {
    std::cerr << "ERROR:filesystem:create_symlink: target path does not exist\n";
    return 1;
  }
  if(link.empty()) {
    std::cerr << "ERROR:filesystem:create_symlink: link path must not be empty\n";
    return 1;
  }

#ifdef __MINGW32__
  // C++ filesystem doesn't work for create_symlink with MinGW, but this C method does work
  return fs_win32_create_symlink(target.c_str(), link.c_str());
#endif

  std::error_code ec;

  if (fs_is_dir(target)) {
    fs::create_directory_symlink(target, link, ec);
  }
  else {
    fs::create_symlink(target, link, ec);
  }
  if(ec) {
    std::cerr << "ERROR:filesystem:create_symlink: " << ec.message() << " " << ec.value() << "\n";
    return ec.value();
  }

  return 0;
}

int fs_create_directories(const char* path)
{
  return fs_create_directories(std::string(path));
}

int fs_create_directories(std::string path)
{
  if(path.empty()) {
    std::cerr << "ERROR:filesystem:mkdir:create_directories: cannot mkdir empty directory name\n";
    return 1;
  }

  std::error_code ec;

  auto s = fs::status(path, ec);
  if(s.type() != fs::file_type::not_found){
    if(ec) {
      std::cerr << "ERROR:filesystem:create_directories:status: " << ec.message() << "\n";
      return ec.value();
    }
  }

  if(fs::exists(s)) {
    if(fs_is_dir(path)) return 0;

    std::cerr << "ERROR:filesystem:mkdir:create_directories: " << path << " already exists but is not a directory\n";
    return 1;
  }

  auto ok = fs::create_directories(path, ec);
  if(ec) {
    std::cerr << "ERROR:filesystem:create_directories: " << ec.message() << "\n";
    return ec.value();
  }

  if( !ok ) {
    // old MacOS return != 0 even if directory was created
    if(fs_is_dir(path)) {
      return 0;
    }
    else
    {
      std::cerr << "ERROR:filesystem:mkdir:create_directories: " << path << " could not be created\n";
      return 1;
    }
  }

  return 0;
}


size_t fs_root(const char* path, char* result, size_t buffer_size)
{
  return fs_str2char(fs_root(std::string(path)), result, buffer_size);
}

std::string fs_root(std::string path)
{
  fs::path p(path);
  return p.root_path().generic_string();
}


bool fs_exists(const char* path)
{
  if(!path)
    return false;
  return fs_exists(std::string(path));
}

bool fs_exists(std::string path)
{
  if(fs_is_reserved(path))
    return true;

  std::error_code ec;
  auto e = fs::exists(path, ec);

  if(ec) {
    std::cerr << "ERROR:ffilesystem:exists: " << ec.message() << "\n";
    return false;
  }

  return e;
}


bool fs_is_absolute(const char* path)
{
  return fs_is_absolute(std::string(path));
}

bool fs_is_absolute(std::string path)
{
  fs::path p(path);
  return p.is_absolute();
}

bool fs_is_char_device(const char* path)
{
  // special POSIX file character device like /dev/null
  return fs_is_char_device(std::string(path));
}

bool fs_is_char_device(std::string path)
{
  if(path.empty())
    return false;

  std::error_code ec;
  bool e = fs::is_character_file(path, ec);
  if(ec) {
    std::cerr << "ERROR:filesystem:is_char_device: " << ec.message() << "\n";
    return false;
  }

  return e;
}


bool fs_is_dir(const char* path)
{
  return fs_is_dir(std::string(path));
}

bool fs_is_dir(std::string path)
{
  if (path.empty())
    return false;

  fs::path p(path);

#ifdef _WIN32
  if (p.root_name() == p)
    return true;
#endif

  std::error_code ec;
  bool e = fs::is_directory(p, ec);
  if(ec) {
    std::cerr << "ERROR:filesystem:is_dir: " << ec.message() << " " << p << "\n";
    return false;
  }

  return e;
}


bool fs_is_exe(const char* path)
{
  return fs_is_exe(std::string(path));
}

bool fs_is_exe(std::string path)
{
  if (!fs_is_file(path))
    return false;

  auto s = fs::status(path);

  auto i = s.permissions() & (fs::perms::owner_exec | fs::perms::group_exec | fs::perms::others_exec);
  return i != fs::perms::none;
}


bool fs_is_file(const char* path)
{
  return fs_is_file(std::string(path));
}

bool fs_is_file(std::string path)
{
  if(path.empty())
    return false;
  if(fs_is_reserved(path))
    return false;

  std::error_code ec;

  bool e = fs::is_regular_file(path, ec);
  if(ec) {
    std::cerr << "ERROR:filesystem:is_file: " << ec.message() << "\n";
    return false;
  }
  return e;
}


bool fs_is_reserved(const char* path)
{
  return fs_is_reserved(std::string(path));
}

bool fs_is_reserved(std::string path)
// https://learn.microsoft.com/en-gb/windows/win32/fileio/naming-a-file#naming-conventions
{

#ifndef _WIN32
  return false;
#endif

  if (path.empty())
    return false;

  std::set<std::string> reserved {
      "CON", "PRN", "AUX", "NUL",
      "COM0", "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9",
      "LPT0", "LPT1", "LPT2", "LPT3", "LPT4", "LPT5", "LPT6", "LPT7", "LPT8", "LPT9"};

  std::transform(path.begin(), path.end(), path.begin(), ::toupper);

#if __cplusplus >= 202002L
  return reserved.contains(path);
#else
  return reserved.find(path) != reserved.end();
#endif
}


bool fs_remove(const char* path)
{
  return fs_remove(std::string(path));
}

bool fs_remove(std::string path)
{
  std::error_code ec;

  bool e = fs::remove(path, ec);

  if(ec) {
    std::cerr << "ERROR:filesystem:remove: " << path << " " << ec.message() << "\n";
    return false;
  }

  return e;
}

size_t fs_canonical(const char* path, bool strict, char* result, size_t buffer_size)
{
  return fs_str2char(fs_canonical(path, strict), result, buffer_size);
}

std::string fs_canonical(std::string path, bool strict)
{
  // also expands ~

  if (path.empty())
    return {};
  if (fs_is_reserved(path))
    return path;

  std::string ex = fs_expanduser(path);

  if(FS_TRACE) std::cout << "TRACE:canonical: input: " << path << " expanded: " << ex << "\n";

  fs::path p;
  std::error_code ec;
  if(strict){
    p = fs::canonical(ex, ec);
  }
  else {
    p = fs::weakly_canonical(ex, ec);
  }
  if(FS_TRACE) std::cout << "TRACE:canonical: " << p << "\n";

  if(ec) {
    std::cerr << "ERROR:filesystem:canonical: " << ec.message() << "\n";
    return {};
  }

  return p.generic_string();
}


bool fs_equivalent(const char* path1, const char* path2)
{
  return fs_equivalent(std::string(path1), std::string(path2));
}

bool fs_equivalent(std::string path1, std::string path2)
{
  // both paths must exist, or they are not equivalent -- return false
  // any non-regular file is not equivalent to anything else -- return false

  if(path1.empty() || path2.empty())
    return false;

  if(fs_is_reserved(path1) || fs_is_reserved(path2))
    return false;

  path1 = fs_canonical(path1, true);
  path2 = fs_canonical(path2, true);

  std::error_code e1, e2;
  if(fs::is_character_file(path1, e1) || fs::is_character_file(path2, e2) ||
     !fs_exists(path1) || !fs_exists(path2) || e1 || e2) {
      return false;
  }

  bool eqv = fs::equivalent(path1, path2, e1);

  if(e1) {
    std::cerr << "ERROR:filesystem:equivalent: " << e1.message() << "\n";
    return false;
  }

  return eqv;
}


int fs_copy_file(const char* source, const char* dest, bool overwrite)
{
  return fs_copy_file(std::string(source), std::string(dest), overwrite);
}

int fs_copy_file(std::string source, std::string dest, bool overwrite)
{
  if(!fs_is_file(source)) {
    std::cerr << "ERROR:filesystem:copy_file: source path must not be empty\n";
    return 1;
  }
  if(dest.empty()) {
    std::cerr << "ERROR:filesystem:copy_file: destination path must not be empty\n";
    return 1;
  }

  auto opt = fs::copy_options::none;

  if (overwrite) {
// WORKAROUND: Windows MinGW GCC 11, Intel oneAPI Linux: bug with overwrite_existing failing on overwrite
    if(fs_exists(dest)){
      if (!fs_remove(dest))
        return 1;
    }

    opt |= fs::copy_options::overwrite_existing;
  }

  std::error_code ec;
  bool ok = fs::copy_file(source, dest, opt, ec);

  if(ec) {
    std::cerr << "ERROR:filesystem:copy_file: " << ec.message() << "\n";
    return ec.value();
  }

  if( !ok ) {
    if(fs_is_file(dest)) {
      return 0;
    }
    else
    {
      std::cerr << "ERROR:filesystem:copy_file: " << dest << " could not be created\n";
      return 1;
    }
  }

  return 0;
}


size_t fs_relative_to(const char* to, const char* from, char* result, size_t buffer_size)
{
  return fs_str2char(fs_relative_to(to, from), result, buffer_size);
}

std::string fs_relative_to(std::string to, std::string from)
{
  // pure lexical operation

  // undefined case, avoid bugs with MacOS
  if (to.empty() || from.empty())
    return {};

  fs::path tp(to), fp(from);
  // cannot be relative, avoid bugs with MacOS
  if(tp.is_absolute() != fp.is_absolute())
    return {};

  // Windows special case for reserved paths
  if(fs_is_reserved(to) || fs_is_reserved(from)){
    if(to == from)
      return ".";
    return {};
  }

  std::error_code ec;
  auto r = fs::relative(tp, fp, ec);

  if(ec) {
    std::cerr << "ERROR:filesystem:relative_to: " << ec.message() << "\n";
    return {};
  }

  return r.generic_string();
}


bool fs_touch(const char* path)
{
  return fs_touch(std::string(path));
}

bool fs_touch(std::string path)
{
  fs::path p(path);
  std::error_code ec;

  auto s = fs::status(p, ec);
  if(s.type() != fs::file_type::not_found){
    if(ec) {
      std::cerr << "ERROR:filesystem:touch:status: " << ec.message() << ": " << p << "\n";
      return false;
    }
  }

  if (fs::exists(s) && !fs::is_regular_file(s)){
    std::cerr << "ERROR:filesystem:touch: " << p << " exists, but is not a regular file\n";
    return false;
  }

  if(fs::is_regular_file(s)) {

    if ((s.permissions() & fs::perms::owner_write) == fs::perms::none){
      std::cerr << "ERROR:filesystem:touch: " << p << " is not writable\n";
      return false;
    }

    fs::last_write_time(p, fs::file_time_type::clock::now(), ec);
    if(ec) {
      std::cerr << "filesystem:touch:last_write_time: " << p << " modtime was not updated: " << ec.message() << "\n";
      return false;
    }
    return true;
  }

  std::ofstream ost;
  ost.open(p, std::ios_base::out);
  if(!ost.is_open()){
    std::cerr << "ERROR:filesystem:touch:open: " << p << " could not be created\n";
    return false;
  }
  ost.close();
  // ensure user can access file, as default permissions may be mode 600 or such
  fs::permissions(p, fs::perms::owner_read | fs::perms::owner_write, fs::perm_options::add, ec);

  if(ec) {
    std::cerr << "ERROR:filesystem:touch:permissions: " << ec.message() << "\n";
    return false;
  }

  return fs::is_regular_file(p);
}


size_t fs_get_tempdir(char* path, size_t buffer_size)
{
  return fs_str2char(fs_get_tempdir(), path, buffer_size);
}

std::string fs_get_tempdir()
{
  std::error_code ec;
  auto r = fs::temp_directory_path(ec);
  if(ec) {
    std::cerr << "ERROR:filesystem:get_tempdir: " << ec.message() << "\n";
    return {};
  }
  return r.generic_string();
}


uintmax_t fs_file_size(const char* path)
{
  return fs_file_size(std::string(path));
}

uintmax_t fs_file_size(std::string path)
{
  // need to check is_regular_file for MSVC/Intel Windows
  if (!fs_is_file(path)) {
    std::cerr << "ERROR:filesystem:file_size: " << path << " is not a regular file\n";
    return 0;
  }

  std::error_code ec;
  auto fsize = fs::file_size(path, ec);
  if (ec) {
    std::cerr << "ERROR:filesystem:file_size: " << path << " could not get file size: " << ec.message() << "\n";
    return 0;
  }

  return fsize;
}


uintmax_t fs_space_available(const char* path)
{
  return fs_space_available(std::string(path));
}

uintmax_t fs_space_available(std::string path)
{
  // filesystem space available for device holding path

  // necessary for MinGW; seemed good choice for all platforms
  if(!fs_exists(path)){
    std::cerr << "ERROR:filesystem:space_available: " << path << " does not exist\n";
    return 0;
  }

  std::error_code ec;
  auto si = fs::space(path, ec);
  if(ec){
    std::cerr << "ERROR:ffilesystem:space_available " << ec.message() << "\n";
    return 0;
  }

  return static_cast<std::intmax_t>(si.available);
}


size_t fs_get_cwd(char* path, size_t buffer_size)
{
  return fs_str2char(fs_get_cwd(), path, buffer_size);
}

std::string fs_get_cwd()
{
  std::error_code ec;
  auto r = fs::current_path(ec);
  if(ec) {
    std::cerr << "ERROR:filesystem:get_cwd: " << ec.message() << "\n";
    return {};
  }
  return r.generic_string();
}


size_t fs_get_homedir(char* path, size_t buffer_size)
{
  return fs_str2char(fs_get_homedir(), path, buffer_size);
}

std::string fs_get_homedir()
{
#ifdef _WIN32
  auto k = "USERPROFILE";
#else
  auto k = "HOME";
#endif

  auto r = std::getenv(k);

  if(!r) {
    std::cerr << "ERROR:filesystem:get_homedir: " << k << " is not defined\n";
    return {};
  }

  return fs_normal(r);
}

size_t fs_expanduser(const char* path, char* result, size_t buffer_size)
{
  return fs_str2char(fs_expanduser(std::string(path)), result, buffer_size);
}

std::string fs_expanduser(std::string path)
{
  if (path.empty())
    return {};

  if(path.front() != '~')
    return fs_normal(path);

  std::string h = fs_get_homedir();
  if (h.empty())
    return fs_normal(path);

  // std::cout << "TRACE:expanduser: path(home) " << home << "\n";

// drop duplicated separators
// NOT .lexical_normal to handle "~/.."
  std::regex r("/{2,}");

  path = std::regex_replace(fs_as_posix(path), r, "/");

if(FS_TRACE) std::cout << "TRACE:expanduser: path deduped " << path << "\n";

  if (path.length() < 3)
    return h;
    // ~ alone

  fs::path home(h);

  return fs_normal((home / path.substr(2)).generic_string());
}


bool fs_chmod_exe(const char* path, bool executable)
{
  // make path file owner executable or not
  return fs_chmod_exe(std::string(path), executable);
}

bool fs_chmod_exe(std::string path, bool executable)
{
  if(!fs_is_file(path)) {
    std::cerr << "ERROR:ffilesystem:chmod_exe: " << path << " is not a regular file\n";
    return false;
  }

  std::error_code ec;

  fs::permissions(path, fs::perms::owner_exec,
    (executable) ? fs::perm_options::add : fs::perm_options::remove,
    ec);

  if(ec) {
    std::cerr << "ERROR:filesystem:chmod_exe: " << path << ": " << ec.message() << "\n";
    return false;
  }

  return true;
}


size_t fs_exe_path(char* path, size_t buffer_size)
{
  return fs_str2char(fs_exe_path(), path, buffer_size);
}


std::string fs_exe_path()
{
  // https://stackoverflow.com/a/4031835
  // https://stackoverflow.com/a/1024937

  char* buf = new char[FS_MAX_PATH]{};

#ifdef _WIN32
 // https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulefilenamea
  if (GetModuleFileName(NULL, buf, FS_MAX_PATH) == 0){
    std::cerr << "ERROR:ffilesystem:exe_path: GetModuleFileName failed\n";
    delete [] buf;
    return {};
  }
#elif defined(__linux__) || defined(__CYGWIN__)
  // https://man7.org/linux/man-pages/man2/readlink.2.html
  size_t L = readlink("/proc/self/exe", buf, FS_MAX_PATH);
  if (L < 1 || L >= FS_MAX_PATH) {
    std::cerr << "ERROR:ffilesystem:lib_path: readlink failed\n";
    delete[] buf;
    return {};
  }
#elif defined(__APPLE__)
  uint32_t mp = FS_MAX_PATH;
  int r = _NSGetExecutablePath(buf, &mp);
  if (r != 0){
    std::cerr << "ERROR:ffilesystem:lib_path: _NSGetExecutablePath failed: " << r << " " << mp << "\n";
    delete[] buf;
    return {};
  }
#elif defined(__OpenBSD__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__DragonFly__)
  int mib[4];
  mib[0] = CTL_KERN;
  mib[1] = KERN_PROC;
  mib[2] = KERN_PROC_PATHNAME;
  mib[3] = -1;
  size_t cb = sizeof(buf);

  if(sysctl(mib, 4, buf, &cb, NULL, 0) != 0){
    std::cerr << "ERROR:ffilesystem:lib_path: sysctl failed\n";
    delete[] buf;
    return {};
  }
#endif

  std::string s(buf);
  delete [] buf;
  return fs_canonical(s, true);
}

size_t fs_exe_dir(char* path, size_t buffer_size)
{
  return fs_str2char(fs_exe_dir(), path, buffer_size);
}


std::string fs_exe_dir()
{
  char* buf = new char[FS_MAX_PATH]{};

  if(fs_exe_path(buf, FS_MAX_PATH) == 0){
    std::cerr << "ERROR:ffilesystem:exe_dir: fs_exe_path failed\n";
    delete [] buf;
    return {};
  }
  std::string s(buf);
  delete [] buf;

  return fs_parent(s);
}


size_t fs_lib_path(char* path, size_t buffer_size)
{
  return fs_str2char(fs_lib_path(), path, buffer_size);
}

std::string fs_lib_path()
{
#if (defined(_WIN32) || defined(__CYGWIN__)) && defined(FS_DLL_NAME)
  char* buf = new char[FS_MAX_PATH];
 // https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulefilenamea
  if(GetModuleFileName(GetModuleHandle(FS_DLL_NAME), buf, FS_MAX_PATH) == 0){
    std::cerr << "ERROR:ffilesystem:lib_path: GetModuleFileName failed\n";
    delete [] buf;
    return {};
  }
  std::string s(buf);
  delete [] buf;
  return s;
#elif defined(HAVE_DLADDR)
  Dl_info info;

  if (dladdr( (void*)&dl_dummy_func, &info) == 0)
    return {};

  return std::string(info.dli_fname);
#endif

  return {};
}


size_t fs_lib_dir(char* path, size_t buffer_size)
{
  return fs_str2char(fs_lib_dir(), path, buffer_size);
}

std::string fs_lib_dir()
{
  std::string s = fs_lib_path();
  if(s.empty()){
    std::cerr << "ERROR:ffilesystem:fs_lib_dir: fs_lib_path failed\n";
    return {};
  }

  if(FS_TRACE) std::cout << "TRACE:fs_lib_dir: " << s << "\n";

  #ifdef __CYGWIN__
    s = fs_as_cygpath(s);
  #endif

  return fs_parent(s);
}


size_t fs_make_absolute(const char* path, const char* top_path, char* out, size_t buffer_size)
{
  return fs_str2char(fs_make_absolute(std::string(path), std::string(top_path)), out, buffer_size);
}

std::string fs_make_absolute(std::string path, std::string top_path)
{
  std::string out = fs_expanduser(path);

  if (!out.empty() && fs_is_absolute(out))
    return out;

  std::string buf = fs_expanduser(top_path);
  if(buf.empty())
    return out;

  return fs_join(buf, out);
}
