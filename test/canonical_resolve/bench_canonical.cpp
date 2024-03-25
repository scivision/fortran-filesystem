#include <chrono>
#include <algorithm>
#include <iostream>
#include <memory>
#include <functional>

#include "ffilesystem.h"


auto bench_c(int n,
    std::string_view path,
    bool strict,
    std::function<size_t(const char*, bool, char*, size_t)> f
    )
{

size_t Lp = fs_get_max_path();
// warmup
auto t = std::chrono::duration<double>::max();
auto buf = std::make_unique<char[]>(Lp);
std::cout << "WarmupC: " << buf.get() << "\n";

if(size_t L = f(path.data(), strict, buf.get(), Lp); L == 0) [[unlikely]]
    return t;

for (int i = 0; i < n; ++i){
    auto t0 = std::chrono::steady_clock::now();
    f(path.data(), strict, buf.get(), Lp);
    auto t1 = std::chrono::steady_clock::now();
    t = std::min(t, std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0));
}

return t;

}


auto bench_cpp(
    int n,
    std::string_view path,
    bool strict,
    std::function<std::string(std::string_view, bool)> f
    )
{

// warmup
auto h = f(path, strict);
auto t = std::chrono::duration<double>::max();
std::cout << "WarmupCpp: " << h << "\n";
if(h.empty()) [[unlikely]]
{
    std::cerr << "Error: empty\n";
    return t;
}

for (int i = 0; i < n; ++i){
    auto t0 = std::chrono::steady_clock::now();
    h = f(path, strict);
    auto t1 = std::chrono::steady_clock::now();
    t = std::min(t, std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0));
}

return t;
}

int main(int argc, char** argv){

int n = 1000;
if(argc > 1)
    n = std::stoi(argv[1]);

std::string_view path = "~";
if(argc > 2)
    path = argv[2];

bool strict = false;

auto t = bench_c(n, path, strict, fs_canonical);
std::cout << "BenchC: " << n << " x fs_canonical(" << path << "): " << t << "\n";

t = bench_c(n, path, strict, fs_resolve);
std::cout << "BenchC: " << n << " x fs_resolve(" << path << "): " << t << "\n";

#if defined(HAVE_CXX_FILESYSTEM) && HAVE_CXX_FILESYSTEM
t = bench_cpp(n, path, strict, Ffs::canonical);
std::cout << "BenchCpp: " << n << " x canonical(" << path << "): " << t << "\n";

t = bench_cpp(n, path, strict, Ffs::resolve);
std::cout << "BenchCpp: " << n << " x resolve(" << path << "): " << t << "\n";
#endif

return EXIT_SUCCESS;

}
