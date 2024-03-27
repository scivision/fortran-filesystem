#include <iostream>
#include <set>
#include <string>
#include <cstdlib>

#include "ffilesystem.h"
#include "ffilesystem_bench.h"


int main(int argc, char** argv){

int n = 1000;
if(argc > 1)
    n = std::stoi(argv[1]);

std::string_view path = "~";
if(argc > 2)
    path = argv[2];

for (std::set<std::string_view, std::less<>> funcs = {"canonical", "resolve"};
      std::string_view func : funcs) {

  auto t = bench_c(n, path, func);
  print_c(t, n, path, func);

  if(!fs_cpp())
    continue;

  t = bench_cpp(n, path, func);
  print_cpp(t, n, path, func);
}


return EXIT_SUCCESS;

}
