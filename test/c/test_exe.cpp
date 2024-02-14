#include <iostream>
#include <cstdlib>
#include <string>

#include <exception>

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

std::string exe = "test_exe";
std::string noexe = "test_noexe";

// Empty string
if(fs_is_exe(""))
    throw std::runtime_error("test_exe: is_exe('') should be false");

// Non-existent file
if (fs_is_file("not-exist"))
    throw std::runtime_error("test_exe: not-exist-file should not exist.");
if (fs_is_exe("not-exist"))
    throw std::runtime_error("test_exe: not-exist-file cannot be executable");
if(fs_get_permissions("not-exist").length() != 0)
    throw std::runtime_error("test_exe: get_permissions('not-exist') should be empty");

fs_touch(exe);
fs_touch(noexe);

fs_set_permissions(exe, 0, 0, 1);
fs_set_permissions(noexe, 0, 0, -1);

if(fs_is_exe(fs_parent(exe)))
    throw std::runtime_error("test_exe: is_exe() should not detect directory " + fs_parent(exe));

std::string p;

std::cout << "permissions: " << exe << " = " << fs_get_permissions(exe) << "\n";

if (!fs_is_file(exe)){
    std::cerr << "test_exe: " << exe << " is not a file.\n";
    return 77;
}

if (!fs_is_exe(exe))
  throw std::runtime_error("test_exe: " + exe + " is not executable and should be.");

std::cout << "permissions: " << noexe << " = " << fs_get_permissions(noexe) << "\n";

if (!fs_is_file(noexe)){
  std::cerr << "test_exe: " << noexe << " is not a file.\n";
  return 77;
}

if (fs_is_exe(noexe)){
  if(fs_is_windows()){
    std::cerr << "XFAIL:Windows: test_exe: is_exe() did not detect non-executable file " << noexe << " on Windows\n";
  }
  else{
   throw std::runtime_error("test_exe: " + noexe + " is executable and should not be.");
  }
}

// chmod setup

fs_remove(exe);
fs_remove(noexe);

// chmod(true)
fs_touch(exe);
if (!fs_is_file(exe))
    throw std::runtime_error("test_exe: " + exe + " is not a file.");

std::cout << "permissions before chmod(" << exe << ", true)  = " << fs_get_permissions(exe) << "\n";

fs_set_permissions(exe, 0, 0, 1);

p = fs_get_permissions(exe);
std::cout << "permissions after chmod(" << exe << ", true) = " << p << "\n";

if (!fs_is_exe(exe)){
  if(fs_is_windows()){
    std::cerr << "XFAIL:Windows: test_exe: is_exe() did not detect executable file " << exe << " on Windows\n";
  }
  else{
    throw std::runtime_error("test_exe: is_exe() did not detect executable file " + exe);
  }
}

if (!fs_is_windows()){
  if(p[2] != 'x')
    throw std::runtime_error("test_exe: expected POSIX perms for " + exe + " to be 'x' in index 2");
}

// chmod(false)
fs_touch(noexe);
if (!fs_is_file(noexe))
    throw std::runtime_error("test_exe: " + noexe + " is not a file.");

std::cout << "permissions before chmod(" << noexe << ", false)  = " << fs_get_permissions(noexe) << "\n";

fs_set_permissions(noexe, 0, 0, 0);

p = fs_get_permissions(noexe);
std::cout << "permissions after chmod(" << noexe << ",false) = " << p << "\n";

if(!fs_is_windows())
{
  if (fs_is_exe(noexe))
    throw std::runtime_error("test_exe: did not detect non-executable file.");

  if (p[2] != '-')
    throw std::runtime_error("test_exe: expected POSIX perms for " + noexe + " to be '-' in index 2");
}

// test fs_which
if(fs_is_windows()){
    std::string which = fs_which("cmd.exe");
    if(which.length() == 0)
        throw std::runtime_error("test_exe: fs_which('cmd.exe') should return a path");
    std::cout << "fs_which('cmd.exe') = " << which << "\n";
    }
else{
    std::string which = fs_which("ls");
    if(which.length() == 0)
        throw std::runtime_error("test_exe: fs_which('ls') should return a path");
    std::cout << "fs_which('ls') = " << which << "\n";
}

fs_remove(exe);
fs_remove(noexe);

std::cout << "OK: c++ test_exe\n";

return EXIT_SUCCESS;
}
