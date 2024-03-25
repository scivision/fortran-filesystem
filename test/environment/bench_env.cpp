#include <chrono>
#include <algorithm>
#include <memory>
#include <iostream>

#include "ffilesystem.h"


auto bench_c(int n, size_t (*f)(char*, size_t)){

size_t Lp = fs_get_max_path();
// warmup
auto t = std::chrono::duration<double>::max();
auto buf = std::make_unique<char[]>(Lp);
size_t L = f(buf.get(), Lp);
std::cout << "WarmupC: " << buf.get() << "\n";
if(L == 0) [[unlikely]]
    return t;

for (int i = 0; i < n; ++i){
    auto t0 = std::chrono::steady_clock::now();
    L = f(buf.get(), Lp);
    auto t1 = std::chrono::steady_clock::now();
    t = std::min(t, std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0));
}

return t;

}

auto bench_cpp(int n, std::string (*f)()){

// warmup
auto t = std::chrono::duration<double>::max();
auto h = f();
if(h.empty()) [[unlikely]]
    return t;
std::cout << "WarmupCpp: " << h << "\n";

for (int i = 0; i < n; ++i){
    auto t0 = std::chrono::steady_clock::now();
    h = f();
    auto t1 = std::chrono::steady_clock::now();
    t = std::min(t, std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0));
}

return t;
}

int main(int argc, char** argv){

int n = 10000;
if(argc > 1)
    n = std::stoi(argv[1]);

#if defined(HAVE_CXX_FILESYSTEM) && HAVE_CXX_FILESYSTEM
auto t_home = bench_cpp(n, Ffs::get_homedir);
std::cout << "BenchCpp: " << n << " x Ffs::get_homedir(): " << t_home << "\n";
#endif

auto t_home_c = bench_c(n, fs_get_homedir);
std::cout << "BenchC: " << n << " x fs_get_homedir(): " << t_home_c << "\n";


return EXIT_SUCCESS;

}
