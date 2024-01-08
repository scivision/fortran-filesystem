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
#elif defined(__linux__) || defined(__CYGWIN__)
#include <unistd.h>
#endif
// --- end of lib_path, exe_path

#if __has_include(<sys/utsname.h>)
#include <sys/utsname.h>
#endif

static std::string fs_generate_random_alphanumeric_string(std::size_t);

size_t fs_get_max_path(){ return FS_MAX_PATH; };


bool fs_cpp()
{
// tell if fs core is C or C++
  return true;
}

long fs_lang()
{
  return __cplusplus;
}


bool fs_is_admin(){
  // running as admin / root / superuser
#ifdef _WIN32
  BOOL adm = FALSE;
	HANDLE hToken = NULL;
	TOKEN_ELEVATION elevation;
	DWORD dwSize;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
		goto fin;

	if (!GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwSize))
		goto fin;

	adm = elevation.TokenIsElevated;

fin:
  if (hToken)
		CloseHandle(hToken);
	return adm;
#else
  return geteuid() == 0;
#endif
}


int fs_is_wsl() {
#if __has_include(<sys/utsname.h>)
  struct utsname buf;
  if (uname(&buf) != 0)
    return false;

  std::string_view release(buf.release);

  if (std::string_view(buf.sysname) != "Linux")
    return 0;

#ifdef __cpp_lib_starts_ends_with
    if (release.ends_with("microsoft-standard-WSL2"))
      return 2;
    if (release.ends_with("-Microsoft"))
      return 1;
#else
    if (release.find("microsoft-standard-WSL2") != std::string::npos)
      return 2;
    if (release.find("-Microsoft") != std::string::npos)
      return 1;
#endif
#endif

  return 0;
}

static size_t fs_str2char(std::string_view s, char* result, size_t buffer_size)
{
  if(s.length() >= buffer_size){
    result = nullptr;
    std::cerr << "ERROR:ffilesystem: output buffer too small for string: " << s << "\n";
    return 0;
  }

  std::strcpy(result, s.data());
  return std::strlen(result);
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
  #ifdef __VERSION__
    return std::format("Clang {}", __VERSION__);
  #else
    return std::format("Clang {}.{}.{}", __clang_major__, __clang_minor__, __clang_patchlevel__);
  #endif
#elif defined(__GNUC__)
  return std::format("GNU GCC {}.{}.{}", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#elif defined(_MSC_VER)
  return std::format("MSVC {}", _MSC_FULL_VER);
#else
  return {};
#endif

#else
  return {};
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
  std::string s = fs::path(path).lexically_normal().generic_string();
  // remove trailing slash
  if (s.length() > 1 && s.back() == '/')
    s.pop_back();
  return s;
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

  return (fs::path(path) / other).lexically_normal().generic_string();
}


size_t fs_parent(const char* path, char* result, size_t buffer_size)
{
  return fs_str2char(fs_parent(std::string_view(path)), result, buffer_size);
}

std::string fs_parent(std::string_view path)
{
  return fs::path(fs_normal(path)).parent_path().generic_string();
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

#ifdef WIN32_SYMLINK
  DWORD a = GetFileAttributes(path.data());

  return a == INVALID_FILE_ATTRIBUTES
    ? false
    : a & FILE_ATTRIBUTE_REPARSE_POINT;
#endif

  std::error_code ec;
  auto s = fs::symlink_status(path, ec);
  // NOTE: use of symlink_status here like lstat(), else logic is wrong with fs::status()

  return ec ? false : fs::is_symlink(s);
}

int fs_create_symlink(const char* target, const char* link)
{
  try{
    fs_create_symlink(std::string_view(target), std::string_view(link));
    return 0;
  } catch(std::exception& e){
    std::cerr << "ERROR:ffilesystem:create_symlink: " << e.what() << "\n";
    return 1;
  }
}

void fs_create_symlink(std::string_view target, std::string_view link)
{
  if(target.empty())
    throw std::runtime_error("ffilesystem:create_symlink: target path does not exist");
    // confusing program errors if target is "" -- we'd never make such a symlink in real use.

  auto s = fs::status(target);

  if(link.empty())
    throw std::runtime_error("ffilesystem:create_symlink: link path must not be empty");
    // macOS needs empty check to avoid SIGABRT

#ifdef WIN32_SYMLINK
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
    fs_create_directories(std::string_view(path));
    return 0;
  } catch(std::exception& e){
    std::cerr << "ERROR:ffilesystem:create_directories: " << e.what() << "\n";
    return 1;
  }
}

void fs_create_directories(std::string_view path)
{
  auto s = fs::status(path);

  if(fs::exists(s)){
    if(fs::is_directory(s))
       return;
    throw std::runtime_error("ffilesystem:create_directories: already exists but non-directory");
  }
  if(fs::create_directories(path) || fs::is_directory(path))
    return;
  // old MacOS return false even if directory was created

  throw std::runtime_error("ffilesystem:create_directories: could not create directory");
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
  return fs_exists(std::string_view(path));
}

bool fs_exists(std::string_view path)
{
  std::error_code ec;
  auto s = fs::status(path, ec);

  return ec ? false : fs::exists(s);
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
  std::error_code ec;
  auto s = fs::status(path, ec);

  return ec ? false : fs::is_character_file(s);
}


bool fs_is_dir(const char* path)
{
  return fs_is_dir(std::string_view(path));
}

bool fs_is_dir(std::string_view path)
{
  fs::path p(path);

  if (fs_is_windows() && !path.empty() && p.root_name() == p)
    return true;

  std::error_code ec;
  auto s = fs::status(p, ec);
  return ec ? false : fs::is_directory(s);
}


bool fs_is_exe(const char* path)
{
  return fs_is_exe(std::string_view(path));
}

bool fs_is_exe(std::string_view path)
{
  std::error_code ec;

  auto s = fs::status(path, ec);
  if(ec || !fs::is_regular_file(s))
    return false;

  auto i = s.permissions() & (fs::perms::owner_exec | fs::perms::group_exec | fs::perms::others_exec);
  return i != fs::perms::none;
}


bool fs_is_file(const char* path)
{
  return fs_is_file(std::string_view(path));
}

bool fs_is_file(std::string_view path)
{
  std::error_code ec;
  auto s = fs::status(path, ec);

  return ec ? false : fs::is_regular_file(s);
}


bool fs_is_reserved(const char* path)
{
  return fs_is_reserved(std::string_view(path));
}

bool fs_is_reserved(std::string_view path)
// https://learn.microsoft.com/en-gb/windows/win32/fileio/naming-a-file#naming-conventions
{

#ifndef _WIN32
    return false;
    (void) path;
#else
  if (path.empty())
    return false;

  std::set<std::string> reserved {
      "CON", "PRN", "AUX", "NUL",
      "COM0", "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9",
      "LPT0", "LPT1", "LPT2", "LPT3", "LPT4", "LPT5", "LPT6", "LPT7", "LPT8", "LPT9"};

  std::string p(path);

  std::transform(p.begin(), p.end(), p.begin(), ::toupper);

  return reserved.contains(p);
#endif
}


bool fs_remove(const char* path)
{
  try {
    return fs_remove(std::string_view(path));
  } catch(std::exception& e){
    std::cerr << "ERROR:ffilesystem:remove: " << e.what() << "\n";
    return false;
  }
}

bool fs_remove(std::string_view path)
{
  return fs::remove(path);
}

size_t fs_canonical(const char* path, bool strict, char* result, size_t buffer_size)
{
  try{
    return fs_str2char(fs_canonical(std::string_view(path), strict), result, buffer_size);
  } catch(std::exception& e){
    std::cerr << "ERROR:ffilesystem:canonical: " << e.what() << "\n";
    return 0;
  }
}

std::string fs_canonical(std::string_view path, bool strict)
{
  // also expands ~

  if (path.empty())
    return {};
    // need this for macOS otherwise it returns the current working directory instead of empty string

  auto ex = fs::path(fs_expanduser(path));

  if(FS_TRACE) std::cout << "TRACE:canonical: input: " << path << " expanded: " << ex << "\n";

  if (!fs::exists(ex) && !ex.is_absolute())
    // handles differences in ill-defined behaviour of fs::weakly_canonical() on non-existant paths
    // canonical(path, false) is distinct from resolve(path, false) for non-existing paths.
    return ex.generic_string();

  return strict ? fs::canonical(ex).generic_string() : fs::weakly_canonical(ex).generic_string();
}


size_t fs_resolve(const char* path, bool strict, char* result, size_t buffer_size)
{
  try{
    return fs_str2char(fs_resolve(std::string_view(path), strict), result, buffer_size);
  } catch(std::exception& e){
    std::cerr << "ERROR:ffilesystem:resolve: " << e.what() << "\n";
    return 0;
  }
}

std::string fs_resolve(std::string_view path, bool strict)
{
  // expands ~ like canonical
  // empty path returns current working directory, which is distinct from canonical that returns empty string
  if(path.empty())
    return fs_get_cwd();

  auto ex = fs::path(fs_expanduser(path));

  if (!fs::exists(ex) && !ex.is_absolute())
    // handles differences in ill-defined behaviour of fs::weakly_canonical() on non-existant paths
    // canonical(path, false) is distinct from resolve(path, false) for non-existing paths.
    ex = fs_get_cwd() / ex;

  return strict ? fs::canonical(ex).generic_string() : fs::weakly_canonical(ex).generic_string();
}


bool fs_equivalent(const char* path1, const char* path2)
{
  try{
    return fs_equivalent(std::string_view(path1), std::string_view(path2));
  } catch(std::exception& e){
    std::cerr << "ERROR:ffilesystem:equivalent: " << e.what() << "\n";
    return false;
  }
}

bool fs_equivalent(std::string_view path1, std::string_view path2)
{
  // non-existant paths are not equivalent

  return fs::equivalent(fs_expanduser(path1), fs_expanduser(path2));
}


int fs_copy_file(const char* source, const char* dest, bool overwrite)
{
  try{
    fs_copy_file(std::string_view(source), std::string_view(dest), overwrite);
    return 0;
  } catch(std::exception& e){
    std::cerr << "ERROR:ffilesystem:copy_file: " << e.what() << "\n";
    return 1;
  }
}

void fs_copy_file(std::string_view source, std::string_view dest, bool overwrite)
{
  std::string s = fs_canonical(source, true);
  std::string d = fs_canonical(dest, false);

  auto opt = fs::copy_options::none;

  if (overwrite) {
// WORKAROUND: Windows MinGW GCC 11, Intel oneAPI Linux: bug with overwrite_existing failing on overwrite
    if(fs_exists(d))
      fs::remove(d);

    opt |= fs::copy_options::overwrite_existing;
  }

  if(!fs::copy_file(s, d, opt) || fs::is_regular_file(d))
    return;

  throw std::runtime_error("ffilesystem:copy_file: could not copy");
}


size_t fs_relative_to(const char* to, const char* from, char* result, size_t buffer_size)
{
  try{
    return fs_str2char(fs_relative_to(to, from), result, buffer_size);
  } catch(std::exception& e){
    std::cerr << "ERROR:ffilesystem:relative_to: " << e.what() << "\n";
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


size_t fs_which(const char* name, char* result, size_t buffer_size)
{
  try{
    return fs_str2char(fs_which(std::string_view(name)), result, buffer_size);
  } catch(std::exception& e){
    std::cerr << "ERROR:ffilesystem:which: " << e.what() << "\n";
    return 0;
  }
}

std::string fs_which(std::string_view name)
// find full path to executable name on Path
{
  if (name.empty())
    return {};

  std::string n(name);

  if (fs_is_absolute(n))
    return fs_is_exe(n) ? fs_normal(n) : std::string();

  std::string path = std::getenv("PATH");
  if (path.empty()){
    std::cerr << "ERROR:ffilesystem:which: Path environment variable not set\n";
    return {};
  }

  std::string p;

  // Windows gives priority to cwd, so check that first
  if(fs_is_windows()){
    p = fs_join(fs_get_cwd(), n);
    if(fs_is_exe(p))
      return p;
  }

  const char pathsep = fs_is_windows() ? ';' : ':';

  std::string::size_type start = 0;
  std::string::size_type end = path.find_first_of(pathsep, start);

  while (end != std::string::npos) {
    p = fs_join(path.substr(start, end - start), n);
    if (FS_TRACE) std::cout << "TRACE:ffilesystem:which: " << p << "\n";
    if (fs_is_exe(p))
      return p;

    start = end + 1;
    end = path.find_first_of(pathsep, start);
  }
  p = fs_join(path.substr(start), n);
  if(fs_is_exe(p))
    return p;

  return {};
}


bool fs_touch(const char* path)
{
  try{
    fs_touch(std::string_view(path));
    return true;
  } catch(std::exception& e){
    std::cerr << "ERROR:ffilesystem:touch: " << path << " " << e.what() << "\n";
    return false;
  }
}

void fs_touch(std::string_view path)
{
  fs::path p(path);

  auto s = fs::status(p);

  if (fs::exists(s) && !fs::is_regular_file(s))
    throw std::runtime_error("ffilesystem:touch: path exists, but is not a regular file");

  if(fs::is_regular_file(s)) {

    if ((s.permissions() & fs::perms::owner_write) == fs::perms::none)
      throw std::runtime_error("ffilesystem:touch: path is not writable");

    fs::last_write_time(p, fs::file_time_type::clock::now());

    return;
  }

  std::ofstream ost;
  ost.open(p, std::ios_base::out);
  if(!ost.is_open())
    throw std::runtime_error("filesystem:touch:open: file could not be created");

  ost.close();
  // ensure user can access file, as default permissions may be mode 600 or such
  fs::permissions(p, fs::perms::owner_read | fs::perms::owner_write, fs::perm_options::add);

  if(!fs::is_regular_file(p))
    throw std::runtime_error("filesystem:touch: file could not be created");
}


size_t fs_get_tempdir(char* path, size_t buffer_size)
{
  try {
    return fs_str2char(fs_get_tempdir(), path, buffer_size);
  } catch(std::exception& e){
    std::cerr << "ERROR:ffilesystem:get_tempdir: " << e.what() << "\n";
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
    std::cerr << "ERROR:ffilesystem:file_size: " << e.what() << "\n";
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
    std::cerr << "ERROR:ffilesystem:space_available: " << e.what() << "\n";
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
    std::cerr << "ERROR:ffilesystem:get_cwd: " << e.what() << "\n";
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
    std::cerr << "ERROR:ffilesystem:set_cwd: " << e.what() << "\n";
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
    std::cerr << "ERROR:ffilesystem:get_homedir: " << e.what() << "\n";
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
  HANDLE hToken = nullptr;
  if(!OpenProcessToken( GetCurrentProcess(), TOKEN_QUERY, &hToken)){
		CloseHandle(hToken);
    throw std::runtime_error("ffilesystem:get_homedir: OpenProcessToken(GetCurrentProcess): "  + std::system_category().message(GetLastError()));
  }

  bool ok = GetUserProfileDirectoryA(hToken, buf.get(), &L);
  CloseHandle(hToken);
  if (!ok)
    throw std::runtime_error("ffilesystem:get_homedir: GetUserProfileDirectory: "  + std::system_category().message(GetLastError()));

  homedir = fs_normal(std::string(buf.get()));
#else
  const char *h = getpwuid(geteuid())->pw_dir;
  if (!h)
    throw std::runtime_error("ffilesystem:get_homedir: getpwuid: "  + std::system_category().message(errno));
  homedir = fs_normal(std::string(h));
#endif

  return homedir;
}

size_t fs_expanduser(const char* path, char* result, size_t buffer_size)
{
  return fs_str2char(fs_expanduser(std::string_view(path)), result, buffer_size);
}

std::string fs_expanduser(std::string_view path)
{
  if(path.empty())
    return {};
  // cannot call .front() on empty string_view() (MSVC)

  if(path.front() != '~' || (path.length() > 1 && path.substr(0, 2) != "~/"))
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

  fs::path home(h);

  return fs_normal((home / p.substr(2)).generic_string());
}


bool fs_chmod_exe(const char* path, bool executable)
{
  // make path file owner executable or not
  // WINDOWS: DOES NOT WORK  -- sys/stat.h chmod() also does not work.
  try{
    fs_chmod_exe(std::string_view(path), executable);
    return true;
  } catch(std::exception& e){
    std::cerr << "ERROR:ffilesystem:chmod_exe: " << executable << " " << e.what() << "\n";
    return false;
  }
}

void fs_chmod_exe(std::string_view path, bool executable)
{
  if(!fs::is_regular_file(path))
    throw std::runtime_error("fffilesystem:chmod_exe: not a regular file");

  fs::permissions(path, fs::perms::owner_exec,
    executable ? fs::perm_options::add : fs::perm_options::remove);
}


size_t fs_exe_path(char* path, size_t buffer_size)
{
  try{
    return fs_str2char(fs_exe_path(), path, buffer_size);
  } catch (std::exception& e) {
    std::cerr << "ERROR:ffilesystem:exe_path: " << e.what() << "\n";
    return 0;
  }
}


std::string fs_exe_path()
{
  // https://stackoverflow.com/a/4031835
  // https://stackoverflow.com/a/1024937

  auto buf = std::make_unique<char[]>(FS_MAX_PATH);

#ifdef _WIN32
 // https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulefilenamea
  if (!GetModuleFileNameA(nullptr, buf.get(), FS_MAX_PATH))
    throw std::runtime_error("ffilesystem:exe_path: GetModuleFileName failed");
#elif defined(__linux__) || defined(__CYGWIN__)
  // https://man7.org/linux/man-pages/man2/readlink.2.html
  size_t L = readlink("/proc/self/exe", buf.get(), FS_MAX_PATH);
  if (L < 1 || L >= FS_MAX_PATH)
    throw std::runtime_error("ffilesystem:exe_path: readlink failed");
#elif defined(__APPLE__)
  uint32_t mp = FS_MAX_PATH;
  if(_NSGetExecutablePath(buf.get(), &mp))
    throw std::runtime_error("ffilesystem:exe_path: _NSGetExecutablePath failed");
#else
  throw std::runtime_error("ffilesystem:exe_path: not implemented for this platform");
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
  return fs_parent(fs_exe_path());
}


size_t fs_get_permissions(const char* path, char* result, size_t buffer_size)
{
  try{
    return fs_str2char(fs_get_permissions(std::string_view(path)), result, buffer_size);
  } catch(std::exception& e){
    std::cerr << "ERROR:ffilesystem:get_permissions: " << e.what() << "\n";
    return 0;
  }
}

std::string fs_get_permissions(std::string_view path)
{
  using std::filesystem::perms;

  auto s = fs::status(path);
  if (!fs::exists(s))
    return {};

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
  try{
    return fs_str2char(fs_lib_path(), path, buffer_size);
  } catch (std::exception& e) {
    std::cerr << "ERROR:ffilesystem:lib_path: " << e.what() << "\n";
    return 0;
  }
}

std::string fs_lib_path()
{
#if (defined(_WIN32) || defined(__CYGWIN__)) && defined(FS_DLL_NAME)
  auto buf = std::make_unique<char[]>(FS_MAX_PATH);

 // https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulefilenamea
  if(!GetModuleFileNameA(GetModuleHandleA(FS_DLL_NAME), buf.get(), FS_MAX_PATH))
    throw std::runtime_error("ffilesystem:lib_path: GetModuleFileName failed");

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
  return fs_parent(fs_lib_path());
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
    std::cerr << "ERROR:ffilesystem:make_tempdir: " << e.what() << "\n";
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
