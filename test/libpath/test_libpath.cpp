#include <cstdlib>
#include <iostream>
#include <string>

#ifdef _MSC_VER
#include <crtdbg.h>
#endif

#include "ffilesystem.h"

[[noreturn]] void err(std::string_view m){
    std::cerr << "ERROR: " << m << "\n";
    std::exit(EXIT_FAILURE);
}


void test_lib_path(char* path, char* ref){

  std::string binpath = Ffs::lib_path();

  if(binpath.empty())
    err("test_binpath: lib_path should be non-empty: " + binpath);

  if(binpath.find(path) == std::string::npos)
    err("test_binpath: lib_path not found correctly: " + binpath + " does not contain " + ref);

  std::cout << "OK: lib_path: " << binpath << "\n";

  std::string bindir = Ffs::lib_dir();

  std::string p;
  p = Ffs::parent(binpath);
  std::cout << "parent(lib_path): " << p << "\n";

  if(bindir.empty())
    err("test_binpath: lib_dir should be non-empty: " + bindir);

  if(!Ffs::equivalent(bindir, p))
    err("test_binpath_c: lib_dir and parent(lib_path) should be equivalent: " + bindir + " != " + p);

  std::cout << "OK: lib_dir: " << bindir << "\n";
}

int main(int argc, char* argv[])
{
#ifdef _MSC_VER
  _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
  _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
#endif

  if (argc < 3) {
    std::cerr << "ERROR: test_libpath_c: not enough arguments\n";
    return 1;
  }

  if (!atoi(argv[1]))
    err("lib_path: feature not available");

  test_lib_path(argv[2], argv[3]);

  return EXIT_SUCCESS;
}
