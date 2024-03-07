#include <iostream>
#include <cstdlib>
#include <string>
#include <exception>

#include <filesystem>

#include "ffilesystem.h"

#ifdef _MSC_VER
#include <crtdbg.h>
#endif

int main()
{

#ifdef _MSC_VER
  _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
  _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
#endif

  std::string fpath = Ffs::get_cwd();
  std::cout << "current working dir " << fpath << "\n";

  if(!Ffs::exists(fpath))
    throw std::runtime_error("current working dir " + fpath + " does not exist");

  std::string cpath = std::filesystem::current_path().string();

  std::string s = Ffs::normal(cpath);

  if (fpath != s)
    throw std::runtime_error("C cwd " + s + " != Fortran cwd " + fpath);

// --- homedir
  std::string p = Ffs::get_homedir();
  std::cout << "Home directory " << p << "\n";
  if (p != Ffs::expanduser("~"))
    throw std::runtime_error("home dir " + p + " != expanduser('~') " + Ffs::expanduser("~"));

// --- tempdir
  std::string t = Ffs::get_tempdir();
  std::cout << "Temp directory " << t << "\n";
  if (!Ffs::exists(t))
    throw std::runtime_error("Fortran: temp dir " + t + " does not exist");

  std::cout << "OK: C++ environment\n";

  return EXIT_SUCCESS;
}
