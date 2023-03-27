#include <cstdlib>
#include <iostream>
#include <string>
#include <stdexcept>

#include "ffilesystem.h"


int test_exe_path(char* argv[])
{

char bin[MAXP];

fs_exe_path(bin, MAXP);
std::string binpath = bin;
  if (binpath.find(argv[2]) == std::string::npos) {
    std::cerr << "ERROR:test_binpath: exe_path not found correctly: " << binpath << "\n";
    return 1;
  }

std::string bindir = fs_exe_dir();
if(bindir.empty()){
  std::cerr << "ERROR:test_binpath: exe_dir not found correctly: " << bindir << "\n";
  return 1;
}
std::string p = fs_parent(binpath);

if(!fs_equivalent(bindir, p)){
  std::cerr << "ERROR:test_binpath: exe_dir and parent(exe_path) should be equivalent: " << bindir << " != " << p << "\n";
  return 1;
}

std::cout << "OK: exe_path: " << binpath << "\n";
std::cout << "OK: exe_dir: " << bindir << "\n";

return 0;
}

int test_lib_path(char* argv[]){

  int shared = atoi(argv[1]);
  if(!shared){
    std::cerr << "SKIP: lib_path: feature not available\n";
    return 0;
  }

  std::string binpath = fs_lib_path();

  if(binpath.empty()){
    std::cerr << "ERROR:test_binpath: lib_path should be non-empty: " << binpath << "\n";
    return 1;
  }
  if(binpath.find(argv[3]) == std::string::npos){
    std::cerr << "ERROR:test_binpath: lib_path not found correctly: " << binpath << " does not contain " << argv[3] << "\n";
    return 1;
  }

  std::cout << "OK: lib_path: " << binpath << "\n";

  std::string bindir = fs_lib_dir();

  std::string p;
#ifdef __CYGWIN__
  p = fs_parent(fs_as_cygpath(binpath));
#else
  p = fs_parent(binpath);
#endif
  std::cout << "parent(lib_path): " << p << "\n";

  if(bindir.empty()){
    std::cerr << "ERROR:test_binpath: lib_dir should be non-empty: " << bindir << "\n";
    return 1;
  }

  if(!fs_equivalent(bindir, p)){
    std::cerr << "ERROR:test_binpath_c: lib_dir and parent(lib_path) should be equivalent: " << bindir << " != " << p << "\n";
    return 1;
  }

  std::cout << "OK: lib_dir: " << bindir << "\n";

  return 0;
}

int main(int argc, char* argv[]){

  if (argc < 4) {
    std::cerr << "ERROR: test_binpath_c: not enough arguments\n";
    return 1;
  }

  int i = test_exe_path(argv);

  i += test_lib_path(argv);

  return i;
}
