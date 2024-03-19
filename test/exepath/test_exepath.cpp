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


void test_exe_path(char* argv[])
{

std::string exepath = Ffs::exe_path();
if (exepath.find(argv[1]) == std::string::npos)
  err("test_exepath: exe_path not found correctly: " + exepath);

std::cout << "OK: exe_path: " << exepath << "\n";

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

  if (argc < 2) {
    std::cerr << "ERROR: test_exepath_c: not enough arguments\n";
    return 1;
  }

  test_exe_path(argv);

  return EXIT_SUCCESS;
}
