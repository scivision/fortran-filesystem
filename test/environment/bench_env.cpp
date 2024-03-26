#include <iostream>
#include <cstdlib>

#include "ffilesystem.h"
#include "ffilesystem_bench.h"


int main(int argc, char** argv){

int n = 10000;
if(argc > 1)
    n = std::stoi(argv[1]);

if(fs_cpp()){
auto t_home = bench_cpp(n, "", "homedir");
std::cout << "Cpp: " << n << " x homedir(): " << t_home << "\n";
}

auto t_home_c = bench_c(n, "", "homedir");
std::cout << "C: " << n << " x homedir(): " << t_home_c << "\n";


return EXIT_SUCCESS;

}
