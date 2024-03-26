#include <iostream>
#include <cstdlib>
#include <set>

#include "ffilesystem.h"
#include "ffilesystem_bench.h"


int main(int argc, char** argv){

int n = 10000;
if(argc > 1)
    n = std::stoi(argv[1]);

std::string_view path = "~";
if(argc > 2)
    path = argv[2];

for (std::set<std::string_view, std::less<>> funcs = {"expanduser", "normal"};
      std::string_view func : funcs) {

auto t = bench_c(n, path.data(), func);
std::cout << "C: " << n << " x " << func << "(" << path << "): " << t << "\n";

if(fs_cpp()){
t = bench_cpp(n, path, func);
std::cout << "Cpp: " << n << " x " << func << "(" << path << "): " << t << "\n";
}

}

return EXIT_SUCCESS;

}
