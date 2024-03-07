#include <iostream>
#include <cstdlib>
#include <string>
#include <exception>

#ifdef _MSC_VER
#include <crtdbg.h>
#endif

#include "ffilesystem.h"

void test_as_posix(){


  std::string p;

  if(!Ffs::as_posix(p).empty())
    throw std::runtime_error("ERROR:test_as_posix: " + p);

  p = "C:\\my\\path";
  if(Ffs::as_posix(p) != "C:/my/path")
    throw std::runtime_error("ERROR:test_as_posix: " + p);

  std::cout << "OK: as_posix\n";


}

void test_filename()
{

if(Ffs::file_name("") != "")
  throw std::runtime_error("filename empty: " + Ffs::file_name(""));

std::cout << "PASS:filename:empty\n";

if (Ffs::file_name("a/b/c") != "c")
  throw std::runtime_error("file_name failed: " + Ffs::file_name("a/b/c"));

if (Ffs::file_name("a") != "a")
 throw std::runtime_error("file_name idempotent failed: " + Ffs::file_name("a"));

if(Ffs::file_name("file_name") != "file_name")
  throw std::runtime_error("file_name plain filename: " + Ffs::file_name("file_name"));

std::string nr = Ffs::file_name(Ffs::root(Ffs::get_cwd()));
if(!nr.empty())
  throw std::runtime_error("file_name root: " + nr);

if(Ffs::file_name(".file_name") != ".file_name")
  throw std::runtime_error("file_name leading dot filename: " + Ffs::file_name(".file_name"));

if(Ffs::file_name("./file_name") != "file_name")
  throw std::runtime_error("file_name leading dot filename cwd: " + Ffs::file_name("./file_name"));

if(Ffs::file_name("file_name.txt") != "file_name.txt")
  throw std::runtime_error("file_name leading dot filename w/ext");

if(Ffs::file_name("./file_name.txt") != "file_name.txt")
  throw std::runtime_error("file_name leading dot filename w/ext and cwd");

if(Ffs::file_name("../file_name.txt") != "file_name.txt")
  throw std::runtime_error("file_name leading dot filename w/ext up " + Ffs::file_name("../file_name.txt"));

if(fs_is_windows() && Ffs::file_name("c:\\my\\path") != "path")
  throw std::runtime_error("file_name windows: " + Ffs::file_name("c:\\my\\path"));

if(fs_is_windows() && fs_pathsep() != ';')
  throw std::runtime_error("pathsep windows");
if(!fs_is_windows() && fs_pathsep() != ':')
  throw std::runtime_error("pathsep unix");

}


int main() {

#ifdef _MSC_VER
  _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
  _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
#endif

  test_as_posix();
  test_filename();

  return EXIT_SUCCESS;
}
