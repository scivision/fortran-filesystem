#include <iostream>
#include <string>
#include <cstdlib>

#include "ffilesystem.h"
#include "ffilesystem_bench.h"


int main(int argc, char** argv){

int n = 1000;
if(argc > 1)
    n = std::stoi(argv[1]);

std::string_view path = (fs_is_windows()) ? "cmd.exe" : "sh";
if(argc > 2)
    path = argv[2];

auto t = bench_c(n, path.data(), "which");
std::cout << "C: " << n << " x which(" << path << "): " << t << "\n";

if(fs_cpp()){
t = bench_cpp(n, path, "which");
std::cout << "Cpp: " << n << " x which(" << path << "): " << t << "\n";
}

return EXIT_SUCCESS;

}
