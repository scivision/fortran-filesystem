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


void test_lib_path(const char* path, const char* ref){

  std::string binpath = Ffs::lib_path();

  if(binpath.empty())
    err("test_binpath: lib_path should be non-empty: " + binpath);

  if(binpath.find(path) == std::string::npos)
    err("test_binpath: lib_path not found correctly: " + binpath + " does not contain " + ref);

  std::cout << "OK: lib_path: " << binpath << "\n";
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
