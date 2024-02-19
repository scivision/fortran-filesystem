#include <iostream>
#include <cstdlib>
#include <string>

#ifdef _MSC_VER
#include <crtdbg.h>
#endif

#include "ffilesystem.h"

int main(void){

#ifdef _MSC_VER
  _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
  _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
#endif

  std::string r, h;

  r = fs_expanduser("");

  if(!r.empty())
    throw std::runtime_error("expanduser('') != ''  " + r);

  r = fs_expanduser(".");
  if (r != ".")
    throw std::runtime_error("expanduser('.') != '.'");

  r = fs_expanduser("~");
  h = fs_get_homedir();
  if(r != h)
    throw std::runtime_error("expanduser('~') != homedir  => " + r + " != " + h);

  std::cout << "~: " << h << "\n";

  r = fs_expanduser("~/");
  h = fs_get_homedir();
  if (r != h)
    throw std::runtime_error("expanduser('~/') != homedir + '/'  =>  " + r + " != " + h);

  std::cout << "~/: " << h << "\n";

  r = fs_expanduser("~//");
  if (r != h)
    throw std::runtime_error("expanduser('~//') != homedir");

  std::cout << "OK: Cpp expanduser\n";

  return EXIT_SUCCESS;
}
