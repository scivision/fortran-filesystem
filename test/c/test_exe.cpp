#include <iostream>
#include <cstdlib>
#include <string>

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

std::string exe = "dummy.exe", noexe = "dummy.no.exe";

// Empty string
if(fs_is_exe(""))
    throw std::runtime_error("ERROR:test_exe: is_exe('') should be false");
if(fs_get_permissions("").length() != 0)
    throw std::runtime_error("ERROR:test_exe: get_permissions('') should be empty");

// Non-existent file
if (fs_is_file("not-exist"))
    throw std::runtime_error("ERROR:test_exe: not-exist-file should not exist.");
if (fs_is_exe("not-exist"))
    throw std::runtime_error("ERROR:test_exe: not-exist-file cannot be executable");
if(fs_get_permissions("not-exist").length() != 0)
    throw std::runtime_error("ERROR:test_exe: get_permissions('not-exist') should be empty");


// chmod(true)
fs_touch(exe);
if (!fs_is_file(exe))
    throw std::runtime_error("ERROR:test_exe: " + exe + " is not a file.");

std::cout << "permissions before chmod(" << exe << ", true)  = " << fs_get_permissions(exe) << "\n";

fs_chmod_exe(exe, true);

std::string p;
p = fs_get_permissions(exe);
std::cout << "permissions after chmod(" << exe << ", true) = " << p << "\n";

if (!fs_is_exe(exe))
    throw std::runtime_error("ERROR:test_exe: is_exe() did not detect executable file " + exe);

if (!fs_is_windows() && p[2] != 'x')
    throw std::runtime_error("ERROR:test_exe: expected POSIX perms for " + exe + " to be 'x' in index 2");

// chmod(false)
fs_touch(noexe);
if (!fs_is_file(noexe))
    throw std::runtime_error("ERROR:test_exe: " + noexe + " is not a file.");

std::cout << "permissions before chmod(" << noexe << ", false)  = " << fs_get_permissions(noexe) << "\n";

fs_chmod_exe(noexe, false);

p = fs_get_permissions(noexe);
std::cout << "permissions after chmod(" << noexe << ",false) = " << p << "\n";

if(!fs_is_windows())
{
  if (fs_is_exe(noexe))
    throw std::runtime_error("ERROR:test_exe: did not detect non-executable file.");

  if (p[2] != '-')
    throw std::runtime_error("ERROR:test_exe: expected POSIX perms for " + noexe + " to be '-' in index 2");
}


std::cout << "OK: c++ test_exe\n";

return EXIT_SUCCESS;
}
