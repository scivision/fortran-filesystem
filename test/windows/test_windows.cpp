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

int main(int argc, char** argv){

#ifdef _MSC_VER
_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
#endif

std::string long_path;

if(argc < 2)
  long_path = std::getenv("PROGRAMFILES");
else
  long_path = argv[1];

if (long_path.empty())
  err("input is empty");

std::string short_path = Ffs::shortname(long_path);

std::cout << long_path << " => " << short_path << '\n';
if(short_path.empty())
  err("short_path is empty");

std::string long_path2 = Ffs::longname(short_path);

std::cout << short_path << " => " << long_path2 << '\n';
if(long_path2.empty())
  err("long_path is empty");

if (long_path != long_path2)
  err("long_path != long_path2");

return EXIT_SUCCESS;
}
