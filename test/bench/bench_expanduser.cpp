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
print_c(t, n, path, func);

if(fs_cpp()){
t = bench_cpp(n, path, func);
print_cpp(t, n, path, func);
}

}

return EXIT_SUCCESS;

}
