#include <iostream>
#include <cstdlib>
#include <string>

#include <exception>

#include "ffilesystem.h"

#ifdef _MSC_VER
#include <crtdbg.h>
#endif

int main(int argc, char *argv[])
{

#ifdef _MSC_VER
  _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
  _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
#endif

if(argc != 3){
    std::cerr << "Usage: " << argv[0] << " <readable> <not_readable>" << std::endl;
    return EXIT_FAILURE;
}

std::string read(argv[1]);
std::string noread(argv[2]);

std::string p = fs_get_permissions("");
if(p.length() != 0)
    throw std::runtime_error("test_exe: get_permissions('') should be empty, got: " + p);

p = fs_get_permissions(read);
std::cout << "Permissions for " << read << ": " << p << std::endl;

if(p.length() == 0)
    throw std::runtime_error("test_exe: get_permissions('" + read + "') should not be empty");

if(p[0] != 'r')
    throw std::runtime_error("test_exe: " + read + " should be readable");

if(!fs_exists(read))
    throw std::runtime_error("test_exe: " + read + " should exist");

if(!fs_is_file(read))
    throw std::runtime_error("test_exe: " + read + " should be a file");

// for Ffilesystem, even non-readable files "exist" and are "is_file"
p = fs_get_permissions(noread);
std::cout << "Permissions for " << noread << ": " << p << std::endl;

if(p.length() == 0)
    throw std::runtime_error("test_exe: get_permissions('" + noread + "') should not be empty");

if(p.find("r") != std::string::npos){
    std::cerr << "ERROR: test_exe: " << noread << " should not be readable\n";
    return 77;
}
if(!fs_exists(noread))
    throw std::runtime_error("test_exe: " + noread + " should exist");

if(!fs_is_file(noread))
    throw std::runtime_error("test_exe: " + noread + " should be a file");


return EXIT_SUCCESS;
}
