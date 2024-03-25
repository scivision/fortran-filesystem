#include <chrono>
#include <algorithm>
#include <iostream>
#include <memory>
#include <functional>

#include "ffilesystem.h"

auto bench_c(
    int n,
    const char* path,
    std::function<size_t(const char*, char*, size_t)> f
    )
{
size_t Lp = fs_get_max_path();
// warmup
auto t = std::chrono::duration<double>::max();
auto buf = std::make_unique<char[]>(Lp);
std::cout << "WarmupC: " << buf.get() << "\n";

if(size_t L = f(path, buf.get(), Lp); L == 0) [[unlikely]]
  return t;

for (int i = 0; i < n; ++i){
    auto t0 = std::chrono::steady_clock::now();
    f(path, buf.get(), Lp);
    auto t1 = std::chrono::steady_clock::now();
    t = std::min(t, std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0));
}

return t;
}

auto bench_cpp(
    int n,
    std::string_view path,
    std::string (*f)(std::string_view)
    )
{

// warmup
auto h = f(path);
auto t = std::chrono::duration<double>::max();
if(h.empty()) [[unlikely]]
  return t;
std::cout << "WarmupCpp: " << h << "\n";

for (int i = 0; i < n; ++i){
    auto t0 = std::chrono::steady_clock::now();
    h = f(path);
    auto t1 = std::chrono::steady_clock::now();
    t = std::min(t, std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0));
}

return t;
}

int main(int argc, char** argv){

int n = 10000;
if(argc > 1)
    n = std::stoi(argv[1]);

std::string_view path = "~";
if(argc > 2)
    path = argv[2];

auto t = bench_c(n, path.data(), fs_expanduser);
std::cout << "BenchC: " << n << " x fs_expanduser(" << path << "): " << t << "\n";

#if defined(HAVE_CXX_FILESYSTEM) && HAVE_CXX_FILESYSTEM
t = bench_cpp(n, path, Ffs::expanduser);
std::cout << "BenchCpp: " << n << " x expanduser(" << path << "): " << t << "\n";

t = bench_cpp(n, path, Ffs::normal);
std::cout << "BenchCpp: " << n << " x normal(" << path << "): " << t << "\n";
#endif

return EXIT_SUCCESS;

}
