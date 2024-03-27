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

std::string_view func = "which";

auto t = bench_c(n, path.data(), func);
print_c(t, n, path, func);

if(fs_cpp()){
t = bench_cpp(n, path, func);
print_cpp(t, n, path, func);
}

return EXIT_SUCCESS;

}
