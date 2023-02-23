#include <iostream>
#include <cstdlib>
#include <string>
#include <stdexcept>

#include "ffilesystem.h"


void test_exe_path(char* argv[])
{
  char binpath[MAXP], bindir[MAXP], p[MAXP];

  fs_exe_path(binpath, MAXP);

  std::string bp(binpath);

  if (bp.find(argv[2]) == std::string::npos)
    throw std::runtime_error("ERROR:test_binpath: exe_path not found correctly");

  if (fs_exe_dir(bindir, MAXP) == 0)
    throw std::runtime_error("ERROR:test_binpath: exe_dir not found correctly");

  fs_parent(binpath, p, MAXP);

  if (!fs_equivalent(bindir, p))
    throw std::runtime_error("ERROR:test_binpath: exe_dir and parent(exe_path) should be equivalent");

  std::cout << "OK: exe_path: " << binpath << std::endl;
  std::cout << "OK: exe_dir: " << bindir << std::endl;

}


void test_lib_path(char* argv[])
{
  char binpath[MAXP], bindir[MAXP];
  int shared = std::stoi(argv[1]);

  size_t L = fs_lib_path(binpath, MAXP);
  size_t L2 = fs_lib_dir(bindir, MAXP);

  if(!shared) {
    if (L != 0 || L2 != 0)
      throw std::runtime_error("ERROR:test_binpath_cpp: lib_path and lib_dir should be empty length 0");

    std::cout << "SKIPPED: lib_path: due to static library" << std::endl;
    return;
  }

  std::string bp(binpath);

  if(bp.find(argv[3]) == std::string::npos)
    throw std::runtime_error("ERROR:test_binpath: lib_path not found correctly");

  char parent[MAXP];

  fs_parent(binpath, parent, MAXP);

  if(!fs_equivalent(bindir, parent))
    throw std::runtime_error("ERROR:test_binpath: lib_dir and parent(lib_path) should be equivalent");

  std::cout << "OK: lib_path: " << binpath << std::endl;
  std::cout << "OK: lib_dir: " << bindir << std::endl;

}

int main(int argc, char* argv[]){

  if (argc < 4) {
    std::cerr << "ERROR: test_binpath_cpp: not enough arguments" << std::endl;
    return EXIT_FAILURE;
  }

  try{
    test_exe_path(argv);
    test_lib_path(argv);
  }
  catch(std::exception& e){
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
