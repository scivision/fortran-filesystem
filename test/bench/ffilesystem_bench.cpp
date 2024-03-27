#include <chrono>
#include <string>
#include <algorithm>
#include <iostream>
#include <functional>
#include <map>

#include "ffilesystem.h"
#include "ffilesystem_bench.h"


std::chrono::duration<double> bench_c(int n, std::string_view path, std::string_view fname)
{

std::map<std::string_view, std::function<size_t(char*, size_t)>> s_ =
  {
    {"compiler", fs_compiler},
    {"homedir", fs_get_homedir},
    {"cwd", fs_get_cwd},
    {"tempdir", fs_get_tempdir}
  };

std::map<std::string_view, std::function<size_t(const char*, char*, size_t)>> s_s =
  {
    {"expanduser", fs_expanduser},
    {"which", fs_which},
    {"parent", fs_parent},
    {"root", fs_root},
    {"stem", fs_stem},
    {"suffix", fs_suffix},
    {"filename", fs_file_name},
    {"perm", fs_get_permissions},
    {"read_symlink", fs_read_symlink},
    {"normal", fs_normal},
    {"getenv", fs_getenv}
  };

std::map<std::string_view, std::function<size_t(const char*, bool, char*, size_t)>> ssb =
  {
    {"canonical", fs_canonical},
    {"resolve", fs_resolve}
  };


const bool strict = false;

size_t Lp = fs_get_max_path();
// warmup
auto t = std::chrono::duration<double>::max();
size_t L=0;

auto buf = std::make_unique<char[]>(Lp);
if (s_.contains(fname))
  L = s_[fname](buf.get(), Lp);
else if (ssb.contains(fname))
  L = ssb[fname](path.data(), strict, buf.get(), Lp);
else if (s_s.contains(fname))
  L = s_s[fname](path.data(), buf.get(), Lp);
else [[unlikely]]
  {
    std::cerr << "Error: unknown function " << fname << "\n";
    return t;
  }
std::cout << "WarmupC: " << buf.get() << "\n";
if(L == 0) [[unlikely]]
    return t;

for (int i = 0; i < n; ++i){
    auto t0 = std::chrono::steady_clock::now();

    if (s_.contains(fname))
      s_[fname](buf.get(), Lp);
    else if (ssb.contains(fname))
      ssb[fname](path.data(), strict, buf.get(), Lp);
    else if (s_s.contains(fname))
      s_s[fname](path.data(), buf.get(), Lp);

    auto t1 = std::chrono::steady_clock::now();
    t = std::min(t, std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0));
}

return t;
}


void print_c(std::chrono::duration<double> t, int n, std::string_view path, std::string_view func)
{
  std::chrono::nanoseconds ns = std::chrono::duration_cast<std::chrono::nanoseconds>(t);
  double us = ns.count() / 1000.0;
  std::cout << "C: " << n << " x " << func << "(" << path << "): " << us << " us\n";
}

void print_cpp(std::chrono::duration<double> t, int n, std::string_view path, std::string_view func)
{
  std::chrono::nanoseconds ns = std::chrono::duration_cast<std::chrono::nanoseconds>(t);
  double us = ns.count() / 1000.0;
  std::cout << "Cpp: " << n << " x " << func << "(" << path << "): " << us << " us\n";
}



std::chrono::duration<double> bench_cpp(int n, std::string_view path, std::string_view fname)
{

auto t = std::chrono::duration<double>::max();

#if defined(HAVE_CXX_FILESYSTEM) && HAVE_CXX_FILESYSTEM

std::map<std::string_view, std::function<std::string()>> s_ =
  {
    {"compiler", Ffs::compiler},
    {"homedir", Ffs::get_homedir},
    {"cwd", Ffs::get_cwd},
    {"tempdir", Ffs::get_tempdir},
    {"exe_path", Ffs::exe_path},
    {"lib_path", Ffs::lib_path}
  };

std::map<std::string_view, std::function<std::string(std::string_view)>> s_s =
  {
    {"as_posix", Ffs::as_posix},
    {"expanduser", Ffs::expanduser},
    {"which", Ffs::which},
    {"parent", Ffs::parent},
    {"root", Ffs::root},
    {"stem", Ffs::stem},
    {"suffix", Ffs::suffix},
    {"filename", Ffs::file_name},
    {"perm", Ffs::get_permissions},
    {"read_symlink", Ffs::read_symlink},
    {"normal", Ffs::normal},
    {"lexically_normal", Ffs::lexically_normal},
    {"make_preferred", Ffs::make_preferred},
    {"mkdtemp", Ffs::mkdtemp},
    {"shortname", Ffs::shortname},
    {"longname", Ffs::longname},
    {"getenv", Ffs::get_env}
  };

std::map<std::string_view, std::function<std::string(std::string_view, bool)>> ssb =
  {
    {"canonical", Ffs::canonical},
    {"resolve", Ffs::resolve}
  };

const bool strict = false;

// warmup
std::string h;

if (s_.contains(fname))
  h = s_[fname]();
else if (ssb.contains(fname))
  h = ssb[fname](path, strict);
else if (s_s.contains(fname))
  h = s_s[fname](path);
else [[unlikely]]
  {
    std::cerr << "Error: unknown function " << fname << "\n";
    return t;
  }

std::cout << "WarmupCpp: " << h << "\n";
if(h.empty()) [[unlikely]]
{
    std::cerr << "Error: empty\n";
    return t;
}

for (int i = 0; i < n; ++i){
    auto t0 = std::chrono::steady_clock::now();

    if (s_.contains(fname))
      h = s_[fname]();
    else if (ssb.contains(fname))
      h = ssb[fname](path, strict);
    else if (s_s.contains(fname))
      h = s_s[fname](path);

    auto t1 = std::chrono::steady_clock::now();
    t = std::min(t, std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0));
}

#endif

return t;
}
