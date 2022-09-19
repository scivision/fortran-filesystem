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

#include "filesystem.h"


bool is_macos(){
#if __APPLE__
#include "TargetConditionals.h"
#if TARGET_OS_MAC
  return true;
#endif
#endif
return false;
}

bool is_linux() {
#ifdef __linux__
  return true;
#endif
return false;
}

bool is_unix() {
#ifdef __unix__
  return true;
#endif
return false;
}

bool is_windows() {
#ifdef _WIN32
  return true;
#endif
return false;
}


size_t normal(const char* path, char* normalized) {
  // normalize path
  fs::path p(path);

  auto s = p.lexically_normal().string();

  std::replace(s.begin(), s.end(), '\\', '/');
  // Windows separator to Unix separator

  // std::cout << "TRACE: normal: " << path << " => " << s << std::endl;

  std::strcpy(normalized, s.c_str());

  return strlen(normalized);
}


bool sys_posix() {
  char sep[2];

  filesep(sep);
  return sep[0] == '/';
}

void filesep(char* sep) {
  fs::path p("/");

  std::strcpy(sep, p.make_preferred().string().c_str());
}


bool match(const char* path, const char* pattern) {
  std::regex r(pattern);
  return std::regex_search(path, r);
}


size_t file_name(const char* path, char* filename) {
  fs::path p(path);

  std::strcpy(filename, p.filename().string().c_str());
  return strlen(filename);
}


size_t stem(const char* path, char* fstem) {
  fs::path p(path);

  auto fn = p.filename();
  auto s = fn.stem();

  // std::cout << "TRACE:suffix: filename = " << fn << " stem = " << s << std::endl;

  std::strcpy(fstem, s.string().c_str());
  return strlen(fstem);
}


size_t parent(const char* path, char* fparent) {
  fs::path p(path);

  if(p.has_parent_path()){
    std::strcpy(fparent, p.parent_path().string().c_str());
  }
  else{
    std::strcpy(fparent, ".");
  }

  return strlen(fparent);
}


size_t suffix(const char* path, char* fsuffix) {

  fs::path p(path);

  auto f = p.filename();
  auto ext = f.extension();

  //std::cout << "TRACE:suffix: filename = " << f << " suffix = " << ext << std::endl;

  std::strcpy(fsuffix, ext.string().c_str());
  return strlen(fsuffix);
}


size_t with_suffix(const char* path, const char* new_suffix, char* swapped) {

  if( path == nullptr || strlen(path) == 0 )
    return 0;

  fs::path p(path);

  std::strcpy(swapped, p.replace_extension(new_suffix).string().c_str());

  return strlen(swapped);
}


bool is_symlink(const char* path) {
if(!exists(path)) return false;

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

int create_symlink(const char* target, const char* link) {

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
  if(is_dir(target)) {
    return !(CreateSymbolicLink(link, target,
      SYMBOLIC_LINK_FLAG_DIRECTORY | SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE) != 0);
  }
  else {
    return !(CreateSymbolicLink(link, target,
      SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE) != 0);
  }
#endif

  std::error_code ec;

  if (is_dir(target)) {
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
    if(is_dir(path)) return 0;

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
    if(is_dir(path)) {
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


size_t root(const char* path, char* result) {
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

bool exists(const char* path) {
  std::error_code ec;

  auto e = fs::exists(path, ec);

  if(ec) return false;

  return e;
}

bool is_absolute(const char* path) {
  fs::path p(path);
  return p.is_absolute();
}

bool is_file(const char* path) {
  std::error_code ec;

  if (!exists(path)) return false;

  return fs::is_regular_file(path);
}

bool is_dir(const char* path) {
  if(std::strlen(path) == 0) return false;

  fs::path p(path);

#ifdef _WIN32
  if (p.root_name() == p) return true;
#endif

  if (!exists(path)) return false;

  return fs::is_directory(path);

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

size_t canonical(const char* path, bool strict, char* result) {
  // also expands ~

  if( path == nullptr || strlen(path) == 0 )
    return 0;

  char ex[MAXP];
  expanduser(path, ex);

  // std::cout << "TRACE:canonical: input: " << path << " expanded: " << ex << std::endl;

  fs::path p;
  std::error_code ec;

  if(strict){
    p = fs::canonical(ex, ec);
  }
  else {
    p = fs::weakly_canonical(ex, ec);
  }

  // std::cout << "TRACE:canonical: " << p << std::endl;

  if(ec) {
    std::cerr << "ERROR:filesystem:canonical: " << ec.message() << std::endl;
    return 0;
  }

  return normal(p.string().c_str(), result);
}


bool equivalent(const char* path1, const char* path2) {
  // check existance to avoid error if not exist
  fs::path p1(path1);
  fs::path p2(path2);

  std::error_code ec;

  if (! (fs::exists(p1, ec) && fs::exists(p2, ec)) ) return false;

  if(ec) {
    std::cerr << "ERROR:filesystem:equivalent: " << ec.message() << std::endl;
    return false;
  }

  auto e = fs::equivalent(p1, p2, ec);

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

  fs::path d(destination);

  auto opt = fs::copy_options::none;

  std::error_code ec;

  if (overwrite) {

// WORKAROUND: Windows MinGW GCC 11, Intel oneAPI Linux: bug with overwrite_existing failing on overwrite
  if(fs::exists(d, ec)) fs::remove(d, ec);

  if(ec) {
    std::cerr << "ERROR:filesystem:copy_file: " << ec.message() << std::endl;
    return ec.value();
  }

  opt |= fs::copy_options::overwrite_existing;
  }

  auto ok = fs::copy_file(source, d, opt, ec);

  if(ec) {
    std::cerr << "ERROR:filesystem:copy_file: " << ec.message() << std::endl;
    return ec.value();
  }

  if( !ok ) {
    if(is_file(destination)) {
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


size_t relative_to(const char* a, const char* b, char* result) {

  // undefined case, avoid bugs with MacOS
  if( a == nullptr || (strlen(a) == 0) || b == nullptr || (strlen(b) == 0) )
    return 0;

  fs::path a1(a);
  fs::path b1(b);

  // cannot be relative, avoid bugs with MacOS
  if(a1.is_absolute() != b1.is_absolute())
    return 0;

  fs::path r;

  std::error_code ec;

  r = fs::relative(a1, b1, ec);

  if(ec) {
    std::cerr << "ERROR:filesystem:relative_to: " << ec.message() << std::endl;
    return 0;
  }

  return normal(r.string().c_str(), result);
}


bool touch(const char* path) {

  if(strlen(path) == 0) {
    std::cerr << "filesystem:touch: cannot touch empty file name" << std::endl;
    return false;
  }

  fs::path p(path);
  std::error_code ec;

  auto s = fs::status(p, ec);
  if(s.type() != fs::file_type::not_found){
    if(ec) {
      std::cerr << "filesystem:touch:status: " << ec.message() << std::endl;
      return false;
    }
  }

  if (fs::exists(s) && !fs::is_regular_file(s)) return false;

  if(!fs::is_regular_file(s)) {
    std::ofstream ost;
    ost.open(p);
    ost.close();
    // ensure user can access file, as default permissions may be mode 600 or such
    fs::permissions(p, fs::perms::owner_read | fs::perms::owner_write, fs::perm_options::add, ec);
  }
  if(ec) {
    std::cerr << "filesystem:touch:permissions: " << ec.message() << std::endl;
    return false;
  }

  if (!fs::is_regular_file(p, ec)) return false;
  // here p because we want to check the new file
  if(ec) {
    std::cerr << "filesystem:touch:is_regular_file: " << ec.message() << std::endl;
    return false;
  }


  fs::last_write_time(p, fs::file_time_type::clock::now(), ec);
  if(ec) {
    std::cerr << "filesystem:touch:last_write_time: " << path << " was created, but modtime was not updated: " << ec.message() << std::endl;
    return false;
  }

  return true;

}


size_t get_tempdir(char* result) {

  std::error_code ec;

  auto t = fs::temp_directory_path(ec);
  if(ec) {
    std::cerr << "filesystem:get_tempdir: " << ec.message() << std::endl;
    return 0;
  }

  return normal(t.string().c_str(), result);
}


uintmax_t file_size(const char* path) {
  // need to check is_regular_file for MSVC/Intel Windows
  fs::path p(path);
  std::error_code ec;

  if (!fs::is_regular_file(p, ec)) {
    std::cerr << "filesystem:file_size: " << p << " is not a regular file" << std::endl;
    return 0;
  }
  if(ec) {
    std::cerr << "ERROR:filesystem:file_size: " << ec.message() << std::endl;
    return 0;
  }

  auto fsize = fs::file_size(p, ec);
  if (ec) {
    std::cerr << "ERROR:filesystem:file_size: " << p << " could not get file size: " << ec.message() << std::endl;
    return 0;
  }

  return fsize;
}


size_t get_cwd(char* result) {
  std::error_code ec;

  auto c = fs::current_path(ec);
  if(ec) {
    std::cerr << "ERROR:filesystem:get_cwd: " << ec.message() << std::endl;
    return 0;
  }

  return normal(c.string().c_str(), result);
}


bool is_exe(const char* path) {
  fs::path p(path);
  std::error_code ec;

  auto s = fs::status(p, ec);
  if (s.type() == fs::file_type::not_found) return false;
  if(ec) {
    std::cerr << "ERROR:filesystem:is_exe: " << ec.message() << std::endl;
    return false;
  }

  if (!fs::is_regular_file(s)) {
    std::cerr << "filesystem:is_exe: " << p << " is not a regular file" << std::endl;
    return false;
  }

  auto i = s.permissions() & (fs::perms::owner_exec | fs::perms::group_exec | fs::perms::others_exec);
  auto isexe = i != fs::perms::none;

  // std::cout << "TRACE:is_exe: " << p << " " << isexe << std::endl;

  return isexe;
}


size_t get_homedir(char* result) {

char path[MAXP];

#ifdef _WIN32
  auto k = "USERPROFILE";
#else
  auto k = "HOME";
#endif

  std::strcpy(path, std::getenv(k));

  return normal(path, result);
}


size_t expanduser(const char* path, char* result){

  std::string p(path);

  // std::cout << "TRACE:expanduser: path: " << p << " length: " << strlen(path) << std::endl;

  if( path == nullptr || strlen(path) == 0 )
    return 0;

  if(p.front() != '~')
    return normal(path, result);


  char h[MAXP];
  if (!get_homedir(h))
    return normal(path, result);

  fs::path home(h);

  // std::cout << "TRACE:expanduser: path(home) " << home << std::endl;

// drop duplicated separators
  std::regex r("/{2,}");

  std::replace(p.begin(), p.end(), '\\', '/');
  p = std::regex_replace(p, r, "/");

  // std::cout << "TRACE:expanduser: path deduped " << p << std::endl;

  if (p.length() == 1) {
    // ~ alone
    return normal(home.string().c_str(), result);
  }
  else if (p.length() == 2) {
    // ~/ alone
    return normal((home.string() + "/").c_str(), result);
  }

  // std::cout << "TRACE:expanduser: trailing path: " << p1 << std::endl;

  return normal((home / p.substr(2)).string().c_str(), result);
}

bool chmod_exe(const char* path) {
  // make path owner executable, if it's a file

  fs::path p(path);
  std::error_code ec;

  if(!fs::is_regular_file(p, ec)) {
    std::cerr << "filesystem:chmod_exe: " << p << " is not a regular file" << std::endl;
    return false;
  }
  if(ec) {
    std::cerr << "ERROR:filesystem:chmod_exe: " << p << ": " << ec.message() << std::endl;
    return false;
  }

  fs::permissions(p, fs::perms::owner_exec, fs::perm_options::add, ec);

  if(ec) {
    std::cerr << "ERROR:filesystem:chmod_exe: " << p << ": " << ec.message() << std::endl;
    return false;
  }

  return true;

}

bool chmod_no_exe(const char* path) {
  // make path not executable, if it's a file

  fs::path p(path);
  std::error_code ec;

  if(!fs::is_regular_file(p, ec)) {
    std::cerr << "filesystem:chmod_no_exe: " << p << " is not a regular file" << std::endl;
    return false;
  }
  if(ec) {
    std::cerr << "ERROR:filesystem:chmod_no_exe: " << p << ": " << ec.message() << std::endl;
    return false;
  }

  fs::permissions(p, fs::perms::owner_exec, fs::perm_options::remove, ec);

  if(ec) {
    std::cerr << "ERROR:filesystem:chmod_no_exe: " << p << ": " << ec.message() << std::endl;
    return false;
  }

  return true;

}
