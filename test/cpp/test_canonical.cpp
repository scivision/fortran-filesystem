#include <iostream>
#include <cstdlib>
#include <string>
#include <exception>

#ifdef _MSC_VER
#include <crtdbg.h>
#endif

#include "ffilesystem.h"


int main(){

#ifdef _MSC_VER
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
#endif

// -- home directory
std::string home = fs_get_homedir();
if(home.empty())
  throw std::runtime_error("fs_get_homedir() failed");

std::string homex = fs_canonical("~", true);

if (home != homex)
  throw std::runtime_error("fs_canonical(~) != fs_get_homedir()");

std::string homep = fs_parent(home);
if(homep.empty())
  throw std::runtime_error("fs_parent(fs_get_homedir()) failed");

// -- relative dir
std::string homer = fs_canonical("~/..", true);
if(homep != homer)
  throw std::runtime_error("fs_canonical(~/..) != fs_parent(fs_get_homedir()) " + homer + " != " + homep);

// -- relative file
if(fs_is_cygwin())
  // Cygwin can't handle non-existing canonical paths
  return EXIT_SUCCESS;

std::string homef = fs_canonical("~/../not-exist.txt", false);
if(homef.empty())
  throw std::runtime_error("fs_canonical(\"~/../not-exist.txt\") failed");

if (homef.length() <= 13)
  throw std::runtime_error("fs_canonical(\"~/../not-exist.txt\") didn't expand ~  " + homef);

return EXIT_SUCCESS;
}
