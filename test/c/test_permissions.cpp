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

std::string p = fs_get_permissions("");
if(p.length() != 0)
    throw std::runtime_error("test_exe: get_permissions('') should be empty, got: " + p);

std::string read = "readable.txt", noread = "nonreadable.txt", nowrite = "nonwritable.txt";

fs_touch(read);
fs_set_permissions(read, 1, 0, 0);

p = fs_get_permissions(read);
std::cout << "Permissions for " << read << ": " << p << "\n";

if(p.length() == 0)
    throw std::runtime_error("test_exe: get_permissions('" + read + "') should not be empty");

if(!fs_is_readable(read))
    throw std::runtime_error("test_exe: " + read + " should be readable");

if(!fs_exists(read))
    throw std::runtime_error("test_exe: " + read + " should exist");

if(!fs_is_file(read))
    throw std::runtime_error("test_exe: " + read + " should be a file");

// for Ffilesystem, even non-readable files "exist" and are "is_file"
fs_touch(noread);
fs_set_permissions(noread, -1, 0, 0);

p = fs_get_permissions(noread);
std::cout << "Permissions for " << noread << ": " << p << "\n";

if(p.length() == 0)
    throw std::runtime_error("test_exe: get_permissions('" + noread + "') should not be empty");

if(fs_is_readable(noread)){
    std::cerr << "XFAIL: test_exe: " << noread << " should not be readable\n";
} else {

if(!fs_exists(noread))
    throw std::runtime_error("test_exe: " + noread + " should exist");

if(!fs_is_file(noread))
    throw std::runtime_error("test_exe: " + noread + " should be a file");
}
// writable
if(!fs_is_file(nowrite))
  fs_touch(nowrite);
fs_set_permissions(nowrite, 0, -1, 0);

std::cout << "Permissions for " << nowrite << ": " << fs_get_permissions(nowrite) << "\n";

if(fs_is_writable(nowrite)){
    std::cerr << "ERROR: test_exe: " << nowrite << " should not be writable\n";
    return 77;
}

if(!fs_exists(nowrite))
    throw std::runtime_error("test_exe: " + nowrite + " should exist");

if(!fs_is_file(nowrite))
    throw std::runtime_error("test_exe: " + nowrite + " should be a file");

return EXIT_SUCCESS;
}
