// functions from C++ filesystem

// NOTE: this segfaults: std::filesystem::path p(nullptr);

#include <iostream>
#include <algorithm>
#include <array>
#include <functional>
#include <random>
#include <cstring>
#include <string>
#include <fstream>
#include <regex>
#include <set>
#include <cstdint>
#include <cstdlib>
#include <system_error>
#include <exception>
#include <filesystem>

#if __has_include(<format>)
#include <format>
#endif

#include "ffilesystem.h"

// for get_homedir backup method
#ifdef _WIN32
#include <userenv.h>
#else
#include <sys/types.h>
#include <pwd.h>
#include <cerrno>
#include <unistd.h> // for mac too
#endif
// end get_homedir backup method

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

static std::string fs_generate_random_alphanumeric_string(std::size_t);

size_t fs_get_max_path(){ return FS_MAX_PATH; };


bool fs_cpp()
{
// tell if fs core is C or C++
  return true;
}

size_t fs_str2char(std::string_view s, char* result, size_t buffer_size)
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

std::string fs_as_posix(std::string_view path)
{
  // force posix file separator
  std::string p(path);
  std::replace(p.begin(), p.end(), '\\', '/');
  return p;
}


void fs_as_windows(char* path)
{
  std::replace(path, path + std::strlen(path), '/', '\\');
}

std::string fs_as_windows(std::string_view path)
{
  // force windows file seperator
  std::string p(path);
  std::replace(p.begin(), p.end(), '/', '\\');
  return p;
}

std::string fs_as_cygpath(std::string_view path)
{
  // like command line "cygpath --unix"

  std::string p(path);
  std::replace(p.begin(), p.end(), '\\', '/');

  return p[1] == ':' && std::isalpha(p[0])
    ? "/cygdrive/" + p.substr(0, 1) + p.substr(2)
    : p;
}


size_t fs_normal(const char* path, char* result, size_t buffer_size)
{
  return fs_str2char(fs_normal(std::string_view(path)), result, buffer_size);
}

std::string fs_normal(std::string_view path)
{
  return fs::path(path).lexically_normal().generic_string();
}


size_t fs_file_name(const char* path, char* result, size_t buffer_size)
{
  return fs_str2char(fs_file_name(std::string_view(path)), result, buffer_size);
}

std::string fs_file_name(std::string_view path)
{
  return fs::path(path).filename().generic_string();
}


size_t fs_stem(const char* path, char* result, size_t buffer_size)
{
  return fs_str2char(fs_stem(std::string_view(path)), result, buffer_size);
}

std::string fs_stem(std::string_view path)
{
  return fs::path(path).filename().stem().generic_string();
}


size_t fs_join(const char* path, const char* other, char* result, size_t buffer_size)
{
  return fs_str2char(fs_join(std::string_view(path), std::string_view(other)), result, buffer_size);
}

std::string fs_join(std::string_view path, std::string_view other)
{
  if (other.empty())
    return std::string(path);

  return (fs::path(path) / other).generic_string();
}


size_t fs_parent(const char* path, char* result, size_t buffer_size)
{
  return fs_str2char(fs_parent(std::string_view(path)), result, buffer_size);
}

std::string fs_parent(std::string_view path)
{
  return fs::path(path).lexically_normal().parent_path().generic_string();
}


size_t fs_suffix(const char* path, char* result, size_t buffer_size)
{
  return fs_str2char(fs_suffix(std::string_view(path)), result, buffer_size);
}

std::string fs_suffix(std::string_view path)
{
  return fs::path(path).filename().extension().generic_string();
}


size_t fs_with_suffix(const char* path, const char* new_suffix,
                      char* result, size_t buffer_size)
{
  return fs_str2char(fs_with_suffix(std::string_view(path), std::string_view(new_suffix)), result, buffer_size);
}

std::string fs_with_suffix(std::string_view path, std::string_view new_suffix)
{
  return fs::path(path).replace_extension(new_suffix).generic_string();
}


bool fs_is_symlink(const char* path)
{
  return fs_is_symlink(std::string_view(path));
}

bool fs_is_symlink(std::string_view path)
{
  if (path.empty())
    return false;

#ifdef __MINGW32__
  DWORD a = GetFileAttributes(path.data());

  return a == INVALID_FILE_ATTRIBUTES
    ? false
    : a & FILE_ATTRIBUTE_REPARSE_POINT;
#endif

  std::error_code ec;

  auto s = fs::status(path, ec);
  return ec ? false : fs::is_symlink(s);
}

int fs_create_symlink(const char* target, const char* link)
{
  try{
    fs_create_symlink(std::string_view(target), std::string_view(link));
    return 0;
  } catch(std::exception& e){
    std::cerr << "ERROR:filesystem:create_symlink: " << e.what() << "\n";
    return 1;
  }
}

void fs_create_symlink(std::string_view target, std::string_view link)
{
  if(target.empty())
    throw std::runtime_error("filesystem:create_symlink: target path does not exist");
    // confusing program errors if target is "" -- we'd never make such a symlink in real use.

  auto s = fs::status(target);

  if(link.empty())
    throw std::runtime_error("filesystem:create_symlink: link path must not be empty");
    // macOS needs empty check to avoid SIGABRT

#ifdef __MINGW32__
  DWORD p = SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE;

  if(fs::is_directory(s))
    p |= SYMBOLIC_LINK_FLAG_DIRECTORY;

  if (CreateSymbolicLink(link.data(), target.data(), p))
    return;

  DWORD err = GetLastError();
  std::string msg = "filesystem:CreateSymbolicLink";
  if(err == ERROR_PRIVILEGE_NOT_HELD)
    msg += "Enable Windows developer mode to use symbolic links: https://learn.microsoft.com/en-us/windows/apps/get-started/developer-mode-features-and-debugging";

  throw std::runtime_error(msg);
#endif

  fs::is_directory(s)
    ? fs::create_directory_symlink(target, link)
    : fs::create_symlink(target, link);
}

int fs_create_directories(const char* path)
{
  try{
    fs_create_directories(std::string_view(path));\
    return 0;
  } catch(std::exception& e){
    std::cerr << "ERROR:filesystem:create_directories: " << e.what() << "\n";
    return 1;
  }
}

void fs_create_directories(std::string_view path)
{
  if(fs_exists(path)){
    if(fs_is_dir(path))
      return;

     throw std::runtime_error("filesystem:create_directories: already exists but non-directory");
  }

  if(fs::create_directories(path) || fs_is_dir(path))
    return;
  // old MacOS return false even if directory was created

  throw std::runtime_error("filesystem:create_directories: could not create");
}


size_t fs_root(const char* path, char* result, size_t buffer_size)
{
  return fs_str2char(fs_root(std::string_view(path)), result, buffer_size);
}

std::string fs_root(std::string_view path)
{
  fs::path p(path);
  return p.root_path().generic_string();
}


bool fs_exists(const char* path)
{
  if(!path)
    return false;
  return fs_exists(std::string_view(path));
}

bool fs_exists(std::string_view path)
{
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
  return fs_is_absolute(std::string_view(path));
}

bool fs_is_absolute(std::string_view path)
{
  fs::path p(path);
  return p.is_absolute();
}

bool fs_is_char_device(const char* path)
{
  // special POSIX file character device like /dev/null
  return fs_is_char_device(std::string_view(path));
}

bool fs_is_char_device(std::string_view path)
{
  try{
    return fs::is_character_file(path);
  } catch(fs::filesystem_error& e){
    std::cerr << "ERROR:filesystem:is_char_device: " << e.what() << "\n";
    return false;
  }
}


bool fs_is_dir(const char* path)
{
  return fs_is_dir(std::string_view(path));
}

bool fs_is_dir(std::string_view path)
{
  if (path.empty())
    return false;

  fs::path p(path);

  if (fs_is_windows() && p.root_name() == p)
    return true;

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
  return fs_is_exe(std::string_view(path));
}

bool fs_is_exe(std::string_view path)
{
  if (!fs_is_file(path))
    return false;

  auto s = fs::status(path);

  auto i = s.permissions() & (fs::perms::owner_exec | fs::perms::group_exec | fs::perms::others_exec);
  return i != fs::perms::none;
}


bool fs_is_file(const char* path)
{
  return fs_is_file(std::string_view(path));
}

bool fs_is_file(std::string_view path)
{
  if (!fs_exists(path))
    return false;
    // exists() check avoids nuisance warnings when file doesn't exist.

  std::error_code ec;

  bool e = fs::is_regular_file(path, ec);
  if (ec) {
    std::cerr << "ERROR:filesystem:is_file: " << ec.message() << "\n";
    return false;
  }
  return e;
}


bool fs_is_reserved(const char* path)
{
  return fs_is_reserved(std::string_view(path));
}

bool fs_is_reserved(std::string_view path)
// https://learn.microsoft.com/en-gb/windows/win32/fileio/naming-a-file#naming-conventions
{

  if(!fs_is_windows())
    return false;

  if (path.empty())
    return false;

  std::set<std::string> reserved {
      "CON", "PRN", "AUX", "NUL",
      "COM0", "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9",
      "LPT0", "LPT1", "LPT2", "LPT3", "LPT4", "LPT5", "LPT6", "LPT7", "LPT8", "LPT9"};

  std::string p(path);

  std::transform(p.begin(), p.end(), p.begin(), ::toupper);

  bool r;
#if __cplusplus >= 202002L
  r = reserved.contains(p);
  if(FS_TRACE) std::cout << "TRACE:is_reserved: C++20: " << path << ": " << r << "\n";
#else
  r = reserved.find(p) != reserved.end();
  if(FS_TRACE) std::cout << "TRACE:is_reserved: C++: " << path << ": " << r << "\n";
#endif

    return r;
}


bool fs_remove(const char* path)
{
  return fs_remove(std::string_view(path));
}

bool fs_remove(std::string_view path)
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
  return fs_str2char(fs_canonical(std::string_view(path), strict), result, buffer_size);
}

std::string fs_canonical(std::string_view path, bool strict)
{
  // also expands ~

  if (path.empty())
    return {};

  auto ex = fs_expanduser(path);

  if(FS_TRACE) std::cout << "TRACE:canonical: input: " << path << " expanded: " << ex << "\n";

  std::error_code ec;
  fs::path p = strict
    ? fs::canonical(ex, ec)
    : fs::weakly_canonical(ex, ec);

  if(FS_TRACE) std::cout << "TRACE:canonical: " << p << "\n";

  if(ec) {
    std::cerr << "ERROR:filesystem:canonical: " << ec.message() << "\n";
    return {};
  }

  return p.generic_string();
}


bool fs_equivalent(const char* path1, const char* path2)
{
  return fs_equivalent(std::string_view(path1), std::string_view(path2));
}

bool fs_equivalent(std::string_view path1, std::string_view path2)
{
  // both paths must exist, or they are not equivalent -- return false
  // any non-regular file is not equivalent to anything else -- return false

  if(path1.empty() || path2.empty())
    return false;

  std::string p1 = fs_canonical(path1, true);
  std::string p2 = fs_canonical(path2, true);

  if (FS_TRACE) std::cout << "TRACE:equivalent: " << p1 << " " << p2 << "\n";

  std::error_code e1, e2;
  if(p1.empty() || p2.empty() ||
     fs::is_character_file(p1, e1) || fs::is_character_file(p2, e2) ||
     !fs_exists(p1) || !fs_exists(p2) || e1 || e2) {
      return false;
  }

  bool eqv = fs::equivalent(p1, p2, e1);

  if(e1) {
    std::cerr << "ERROR:filesystem:equivalent: " << e1.message() << "\n";
    return false;
  }

  return eqv;
}


int fs_copy_file(const char* source, const char* dest, bool overwrite)
{
  return fs_copy_file(std::string_view(source), std::string_view(dest), overwrite);
}

int fs_copy_file(std::string_view source, std::string_view dest, bool overwrite)
{
  std::string s = fs_canonical(source, true);
  std::string d = fs_canonical(dest, false);

  if(s.empty() || !fs_is_file(s)) {
    std::cerr << "ERROR:filesystem:copy_file: source path must not be empty\n";
    return 1;
  }
  if(d.empty()) {
    std::cerr << "ERROR:filesystem:copy_file: destination path must not be empty\n";
    return 1;
  }

  auto opt = fs::copy_options::none;

  if (overwrite) {
// WORKAROUND: Windows MinGW GCC 11, Intel oneAPI Linux: bug with overwrite_existing failing on overwrite
    if(fs_exists(d)){
      if (!fs_remove(d))
        return 1;
    }

    opt |= fs::copy_options::overwrite_existing;
  }

  std::error_code ec;
  bool ok = fs::copy_file(s, d, opt, ec);

  if(ec) {
    std::cerr << "ERROR:filesystem:copy_file: " << ec.message() << "\n";
    return ec.value();
  }

  if( !ok ) {
    if(fs_is_file(d)) {
      return 0;
    }
    else
    {
      std::cerr << "ERROR:filesystem:copy_file: " << d << " could not be created\n";
      return 1;
    }
  }

  return 0;
}


size_t fs_relative_to(const char* to, const char* from, char* result, size_t buffer_size)
{
  try{
    return fs_str2char(fs_relative_to(to, from), result, buffer_size);
  } catch(std::exception& e){
    std::cerr << "ERROR:filesystem:relative_to: " << e.what() << "\n";
    return 0;
  }
}

std::string fs_relative_to(std::string_view to, std::string_view from)
{
  // pure lexical operation

  // undefined case, avoid bugs with MacOS
  if (to.empty() || from.empty())
    return {};

  fs::path tp(to), fp(from);
  // cannot be relative, avoid bugs with MacOS
  if(tp.is_absolute() != fp.is_absolute())
    return {};

  return fs::relative(tp, fp).generic_string();
}


bool fs_touch(const char* path)
{
  try{
    return fs_touch(std::string_view(path));
  } catch(std::exception& e){
    std::cerr << "ERROR:filesystem:touch: " << path << " " << e.what() << "\n";
    return false;
  }
}

bool fs_touch(std::string_view path)
{
  fs::path p(path);

  auto s = fs::status(p);

  if (fs::exists(s) && !fs::is_regular_file(s))
    throw std::runtime_error("filesystem:touch: path exists, but is not a regular file");

  if(fs::is_regular_file(s)) {

    if ((s.permissions() & fs::perms::owner_write) == fs::perms::none)
      throw std::runtime_error("filesystem:touch: path is not writable");

    fs::last_write_time(p, fs::file_time_type::clock::now());

    return true;
  }

  std::ofstream ost;
  ost.open(p, std::ios_base::out);
  if(!ost.is_open())
    throw std::runtime_error("filesystem:touch:open: file could not be created");

  ost.close();
  // ensure user can access file, as default permissions may be mode 600 or such
  fs::permissions(p, fs::perms::owner_read | fs::perms::owner_write, fs::perm_options::add);

  return fs::is_regular_file(p);
}


size_t fs_get_tempdir(char* path, size_t buffer_size)
{
  try {
    return fs_str2char(fs_get_tempdir(), path, buffer_size);
  } catch(std::exception& e){
    std::cerr << "ERROR:filesystem:get_tempdir: " << e.what() << "\n";
    return 0;
  }
}

std::string fs_get_tempdir()
{
  return fs::temp_directory_path().generic_string();
}


uintmax_t fs_file_size(const char* path)
{
  try{
    return fs_file_size(std::string_view(path));
  } catch(std::exception& e){
    std::cerr << "ERROR:filesystem:file_size: " << e.what() << "\n";
    return 0;
  }
}

uintmax_t fs_file_size(std::string_view path)
{
  return fs::file_size(path);
}


uintmax_t fs_space_available(const char* path)
{
  try{
    return fs_space_available(std::string_view(path));
  } catch(std::exception& e){
    std::cerr << "ERROR:filesystem:space_available: " << e.what() << "\n";
    return 0;
  }
}

uintmax_t fs_space_available(std::string_view path)
{
  // filesystem space available for device holding path

  return static_cast<std::intmax_t>(fs::space(path).available);
}


size_t fs_get_cwd(char* path, size_t buffer_size)
{
  try{
    return fs_str2char(fs_get_cwd(), path, buffer_size);
  } catch(std::exception& e){
    std::cerr << "ERROR:filesystem:get_cwd: " << e.what() << "\n";
    return 0;
  }
}

std::string fs_get_cwd()
{
  return fs::current_path().generic_string();
}


bool fs_set_cwd(const char *path)
{
  try{
    fs::current_path(path);
    return true;
  } catch (std::exception& e) {
    std::cerr << "ERROR:filesystem:set_cwd: " << e.what() << "\n";
    return false;
  }

}

void fs_set_cwd(std::string_view path)
{
  fs::current_path(path);
}


size_t fs_get_homedir(char* path, size_t buffer_size)
{
  try{
    return fs_str2char(fs_get_homedir(), path, buffer_size);
  } catch (std::exception& e) {
    std::cerr << "ERROR:filesystem:get_homedir: " << e.what() << "\n";
    return 0;
  }
}

std::string fs_get_homedir()
{

  auto r = std::getenv(fs_is_windows() ? "USERPROFILE" : "HOME");
  if (r && std::strlen(r) > 0)
    return fs_normal(r);

  std::string homedir;
#ifdef _WIN32
  // works on MSYS2, MSVC, oneAPI.
  DWORD L = FS_MAX_PATH;
  auto buf = std::make_unique<char[]>(L);
  // process with query permission
  HANDLE hToken = 0;
  if(!OpenProcessToken( GetCurrentProcess(), TOKEN_QUERY, &hToken))
    throw std::runtime_error("OpenProcessToken(GetCurrentProcess): "  + std::system_category().message(GetLastError()));

  bool ok = GetUserProfileDirectoryA(hToken, buf.get(), &L);
  CloseHandle(hToken);
  if (!ok)
    throw std::runtime_error("GetUserProfileDirectory: "  + std::system_category().message(GetLastError()));

  homedir = std::string(buf.get());
#else
  const char *h = getpwuid(geteuid())->pw_dir;
  if (!h)
    throw std::runtime_error("getpwuid: "  + std::system_category().message(errno));
  homedir = std::string(h);
#endif

  return homedir;
}

size_t fs_expanduser(const char* path, char* result, size_t buffer_size)
{
  return fs_str2char(fs_expanduser(std::string_view(path)), result, buffer_size);
}

std::string fs_expanduser(std::string_view path)
{
  if(path.front() != '~')
    return fs_normal(path);

  std::string h = fs_get_homedir();
  if (h.empty())
    return fs_normal(path);

  // std::cout << "TRACE:expanduser: path(home) " << home << "\n";

// drop duplicated separators
// NOT .lexical_normal to handle "~/.."
  std::regex r("/{2,}");

  std::string p(path);
  p = std::regex_replace(p, r, "/");

  if(FS_TRACE) std::cout << "TRACE:expanduser: path deduped " << p << "\n";

  if (p.length() < 3)
    return h;
    // ~ alone

  fs::path home(h);

  return fs_normal((home / p.substr(2)).generic_string());
}


bool fs_chmod_exe(const char* path, bool executable)
{
  // make path file owner executable or not
  try{
    fs_chmod_exe(std::string_view(path), executable);
    return true;
  } catch(std::exception& e){
    std::cerr << "ERROR:filesystem:chmod_exe: " << e.what() << "\n";
    return false;
  }
}

void fs_chmod_exe(std::string_view path, bool executable)
{
  if(!fs_is_file(path))
    throw std::runtime_error("ffilesystem:chmod_exe: not a regular file");

  fs::permissions(path, fs::perms::owner_exec,
    executable ? fs::perm_options::add : fs::perm_options::remove);
}


size_t fs_exe_path(char* path, size_t buffer_size)
{
  return fs_str2char(fs_exe_path(), path, buffer_size);
}


std::string fs_exe_path()
{
  // https://stackoverflow.com/a/4031835
  // https://stackoverflow.com/a/1024937

  auto buf = std::make_unique<char[]>(FS_MAX_PATH);

#ifdef _WIN32
 // https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulefilenamea
  if (!GetModuleFileNameA(nullptr, buf.get(), FS_MAX_PATH)){
    std::cerr << "ERROR:ffilesystem:exe_path: GetModuleFileName failed\n";
    return {};
  }
#elif defined(__linux__) || defined(__CYGWIN__)
  // https://man7.org/linux/man-pages/man2/readlink.2.html
  size_t L = readlink("/proc/self/exe", buf.get(), FS_MAX_PATH);
  if (L < 1 || L >= FS_MAX_PATH) {
    std::cerr << "ERROR:ffilesystem:lib_path: readlink failed\n";
    return {};
  }
#elif defined(__APPLE__)
  uint32_t mp = FS_MAX_PATH;
  int r = _NSGetExecutablePath(buf.get(), &mp);
  if (r){
    std::cerr << "ERROR:ffilesystem:lib_path: _NSGetExecutablePath failed: " << r << " " << mp << "\n";
    return {};
  }
#elif defined(__OpenBSD__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__DragonFly__)
  int mib[4];
  mib[0] = CTL_KERN;
  mib[1] = KERN_PROC;
  mib[2] = KERN_PROC_PATHNAME;
  mib[3] = -1;
  size_t cb = sizeof(buf);

  if(sysctl(mib, 4, buf.get(), &cb, nullptr, 0)){
    std::cerr << "ERROR:ffilesystem:lib_path: sysctl failed\n";
    return {};
  }
#endif

  std::string s(buf.get());
  return fs_canonical(s, true);
}

size_t fs_exe_dir(char* path, size_t buffer_size)
{
  return fs_str2char(fs_exe_dir(), path, buffer_size);
}


std::string fs_exe_dir()
{
  auto buf = std::make_unique<char[]>(FS_MAX_PATH);

  if(!fs_exe_path(buf.get(), FS_MAX_PATH)){
    std::cerr << "ERROR:ffilesystem:exe_dir: fs_exe_path failed\n";
    return {};
  }
  std::string s(buf.get());

  return fs_parent(s);
}


size_t fs_get_permissions(const char* path, char* result, size_t buffer_size)
{
  return fs_str2char(fs_get_permissions(std::string_view(path)), result, buffer_size);
}

std::string fs_get_permissions(std::string_view path)
{
  using std::filesystem::perms;

  if (!fs_exists(path))
    return {};
    // exists() check avoids nuisance warnings when file doesn't exist.

  std::error_code ec;
  auto s = fs::status(path, ec);
  if(ec) {
    std::cerr << "ERROR:filesystem:get_permissions: " << ec.message() << "\n";
    return {};
  }

  perms p = s.permissions();

  std::string r = "---------";
  if ((p & perms::owner_read) != perms::none)
    r[0] = 'r';
  if ((p & perms::owner_write) != perms::none)
    r[1] = 'w';
  if ((p & perms::owner_exec) != perms::none)
    r[2] = 'x';
  if ((p & perms::group_read) != perms::none)
    r[3] = 'r';
  if ((p & perms::group_write) != perms::none)
    r[4] = 'w';
  if ((p & perms::group_exec) != perms::none)
    r[5] = 'x';
  if ((p & perms::others_read) != perms::none)
    r[6] = 'r';
  if ((p & perms::others_write) != perms::none)
    r[7] = 'w';
  if ((p & perms::others_exec) != perms::none)
    r[8] = 'x';

  return r;
}


size_t fs_lib_path(char* path, size_t buffer_size)
{
  return fs_str2char(fs_lib_path(), path, buffer_size);
}

std::string fs_lib_path()
{
#if (defined(_WIN32) || defined(__CYGWIN__)) && defined(FS_DLL_NAME)
  auto buf = std::make_unique<char[]>(FS_MAX_PATH);

 // https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulefilenamea
  if(!GetModuleFileNameA(GetModuleHandleA(FS_DLL_NAME), buf.get(), FS_MAX_PATH)){
    std::cerr << "ERROR:ffilesystem:lib_path: GetModuleFileName failed\n";
    return {};
  }
  std::string s(buf.get());
  return s;
#elif defined(HAVE_DLADDR)
  Dl_info info;

  return dladdr( (void*)&dl_dummy_func, &info)
    ? std::string(info.dli_fname)
    : std::string();
#else
  return {};
#endif
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

  if (fs_is_cygwin()) s = fs_as_cygpath(s);

  return fs_parent(s);
}


size_t fs_make_absolute(const char* path, const char* top_path, char* out, size_t buffer_size)
{
  return fs_str2char(fs_make_absolute(std::string_view(path), std::string_view(top_path)), out, buffer_size);
}

std::string fs_make_absolute(std::string_view path, std::string_view top_path)
{
  std::string out = fs_expanduser(path);

  if (!out.empty() && fs_is_absolute(out))
    return out;

  std::string buf = fs_expanduser(top_path);

  return buf.empty() ? out : fs_join(buf, out);
}

// --- mkdtemp

size_t fs_make_tempdir(char* result, size_t buffer_size){
  // Fortran / C / C++ interface function

  std::string tmpdir;
  try{
    tmpdir = fs_make_tempdir("tmp.");
  } catch(fs::filesystem_error& e) {
    std::cerr << "ERROR:filesystem:make_tempdir: " << e.what() << "\n";
    return 0;
  }
  return fs_str2char(tmpdir, result, buffer_size);
}


std::string fs_make_tempdir(std::string prefix)
{
  // make unique temporary directory starting with prefix

  fs::path t;
  size_t Lname = 16;  // arbitrary length for random string

  do {
    t = (fs::temp_directory_path() / (prefix + fs_generate_random_alphanumeric_string(Lname)));
  } while (fs::is_directory(t));

  if (!fs::create_directory(t))
    throw fs::filesystem_error("fs_make_tempdir:mkdir: could not create temporary directory", t, std::error_code(errno, std::system_category()));

  return t.generic_string();

}

// CTAD C++17 random string generator
// https://stackoverflow.com/a/444614

template <typename T = std::mt19937>
static auto fs_random_generator() -> T {
    auto constexpr seed_bytes = sizeof(typename T::result_type) * T::state_size;
    auto constexpr seed_len = seed_bytes / sizeof(std::seed_seq::result_type);
    auto seed = std::array<std::seed_seq::result_type, seed_len>();
    auto dev = std::random_device();
    std::generate_n(begin(seed), seed_len, std::ref(dev));
    auto seed_seq = std::seed_seq(begin(seed), end(seed));
    return T{seed_seq};
}

static std::string fs_generate_random_alphanumeric_string(std::size_t len)
{
    static constexpr auto chars =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    thread_local auto rng = fs_random_generator<>();
    auto dist = std::uniform_int_distribution{{}, std::strlen(chars) - 1};
    auto result = std::string(len, '\0');
    std::generate_n(begin(result), len, [&]() { return chars[dist(rng)]; });
    return result;
}
// --- end mkdtemp
