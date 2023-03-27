// use ffilesystem library from C++

#include <iostream>
#include <cstdlib>
#include <string>

#ifdef _MSC_VER
#include <direct.h>
#else
#include <unistd.h>
#endif

#include <stdexcept>

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

  std::string fpath = fs_get_cwd();
  std::cout << "current working dir " << fpath << "\n";

  if(!fs_exists(fpath))
    throw std::runtime_error("current working dir " + fpath + " does not exist");

  char* cpath = new char[FS_MAX_PATH];
  #ifdef _MSC_VER
  if(!_getcwd(cpath, FS_MAX_PATH))
    throw std::runtime_error("C getcwd failed");
#else
  if(!getcwd(cpath, FS_MAX_PATH))
    throw std::runtime_error("C getcwd failed");
#endif
  std::string s = fs_normal(std::string(cpath));
  delete [] cpath;

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
