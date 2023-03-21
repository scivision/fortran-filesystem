// use ffilesystem library from C++

#include <iostream>
#include <cstdlib>
#include <string>

#ifdef _MSC_VER
#include <direct.h>
#else
#include <unistd.h>
#endif

#include "ffilesystem.h"

int main() {

  char cpath[MAXP];

  std::string fpath = fs_get_cwd();
  std::cout << "Fortran: current working dir " << fpath << "\n";

#ifdef _MSC_VER
    if(_getcwd(cpath, MAXP)  == nullptr)
      return EXIT_FAILURE;
#else
    if(getcwd(cpath, MAXP) == nullptr)
      return EXIT_FAILURE;
#endif

  std::string s = fs_normal(std::string(cpath));

  if (fpath != s) {
    std::cerr << "C cwd " << s << " != Fortran cwd " << fpath << "\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
