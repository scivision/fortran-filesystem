#include <iostream>
#include <cstdlib>

#include "ffilesystem.h"
#include "ffilesystem_bench.h"


int main(int argc, char** argv){

int n = 10000;
if(argc > 1)
    n = std::stoi(argv[1]);

std::string_view func = "homedir";

auto t = bench_c(n, "", func);
print_c(t, n, "", func);

if(fs_cpp()){
t = bench_cpp(n, "", func);
print_cpp(t, n, "", func);
}


return EXIT_SUCCESS;

}
