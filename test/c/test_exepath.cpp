#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>

#ifdef _MSC_VER
#include <crtdbg.h>
#endif

#include "ffilesystem.h"


void test_exe_path(char* argv[])
{
char bin[FS_MAX_PATH];

fs_exe_path(bin, FS_MAX_PATH);
std::string exepath = bin;
  if (exepath.find(argv[1]) == std::string::npos)
    throw std::runtime_error("ERROR:test_exepath: exe_path not found correctly: " + exepath);


std::string bindir = fs_exe_dir();
if(bindir.empty())
  throw std::runtime_error("ERROR:test_exepath: exe_dir not found correctly: " + bindir);

std::string p = fs_parent(exepath);

if(!fs_equivalent(bindir, p))
  throw std::runtime_error("ERROR:test_exepath: exe_dir and parent(exe_path) should be equivalent: " + bindir + " != " + p);

std::cout << "OK: exe_path: " << exepath << "\n";
std::cout << "OK: exe_dir: " << bindir << "\n";

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
