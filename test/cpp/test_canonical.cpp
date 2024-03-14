#include <iostream>
#include <cstdlib>
#include <string>

#ifdef _MSC_VER
#include <crtdbg.h>
#endif

#include "ffilesystem.h"

[[noreturn]] void err(std::string_view m){
    std::cerr << "ERROR: " << m << "\n";
    std::exit(EXIT_FAILURE);
}


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
std::string home = Ffs::get_homedir();
if(home.empty())
  err("get_homedir() failed");

std::string homex = Ffs::canonical("~", true);

if (home != homex)
  err("Ffs::canonical(~) != get_homedir()");

std::string homep = Ffs::parent(home);
if(homep.empty())
  err("Ffs::parent(get_homedir()) failed");

// -- relative dir

if(std::string homer = Ffs::canonical("~/..", true); homep != homer)
  err("Ffs::canonical(~/..) != Ffs::parent(get_homedir()) " + homer + " != " + homep);

// -- relative file
if(fs_is_cygwin())
  // Cygwin can't handle non-existing canonical paths
  return EXIT_SUCCESS;

std::string homef = Ffs::canonical("~/../not-exist.txt", false);
if(homef.empty())
  err("Ffs::canonical(\"~/../not-exist.txt\") failed");

if (homef.length() <= 13)
  err("Ffs::canonical(\"~/../not-exist.txt\") didn't expand ~  " + homef);

return EXIT_SUCCESS;
}
