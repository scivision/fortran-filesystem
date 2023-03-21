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
  std::cout << "current working dir " << fpath << "\n";

  if(!fs_exists(fpath))
    throw std::runtime_error("current working dir " + fpath + " does not exist");

#ifdef _MSC_VER
    if(_getcwd(cpath, MAXP)  == nullptr)
      return EXIT_FAILURE;
#else
    if(getcwd(cpath, MAXP) == nullptr)
      return EXIT_FAILURE;
#endif

  std::string s = fs_normal(std::string(cpath));

  if (fpath != s)
    throw std::runtime_error("C cwd " + s + " != Fortran cwd " + fpath);

// --- homedir
  std::string p = fs_get_homedir();
  std::cout << "Home directory " << p << "\n";
  if (p != fs_expanduser("~"))
    throw std::runtime_error("home dir " + p + " != expanduser('~') " + fs_expanduser("~"));

// --- tempdir
  p = fs_get_tempdir();
  std::cout << "Temp directory " << p << "\n";
  if (!fs_exists(p))
    throw std::runtime_error("Fortran: temp dir " + p + " does not exist");

  return EXIT_SUCCESS;
}
