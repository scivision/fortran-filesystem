#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>

#include <filesystem>

#include <sys/stat.h>

#ifdef _WIN32
#define _WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <sys/un.h>
#endif

#ifdef _MSC_VER
#include <io.h>
#include <crtdbg.h>
#else
#include <unistd.h>
#endif

#include "file_status.h"


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

    auto tempdir = fs::temp_directory_path() / "sandbox";

    std::cout << "Working directory: " << tempdir << std::endl;

    // create files of different kinds
    if (fs::exists(tempdir))
        fs::remove_all(tempdir);
    fs::create_directory(tempdir);

    std::ofstream(tempdir / "file"); // create regular file

    fs::create_directory(tempdir / "dir");

#ifndef _WIN32
    mkfifo((tempdir / "pipe").generic_string().c_str(), 0644);

    // afunix.h didn't help, silently fails
    sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    std::strcpy(addr.sun_path, (tempdir / "sock").generic_string().c_str());
    int fd = socket(PF_UNIX, SOCK_STREAM, 0);
    bind(fd, reinterpret_cast<sockaddr*>(&addr), sizeof addr);
#endif

#ifndef __MINGW32__
    fs::create_symlink(tempdir / "file", tempdir / "symlink");
#endif

    // demo different status accessors
    for(auto it = fs::directory_iterator(tempdir); it != fs::directory_iterator(); ++it)
        print_file_status(*it);

    print_file_status("/dev/null");
    print_file_status("nul");
    print_file_status("NUL");
    print_file_status("/dev/sda");
    print_file_status(tempdir / "no");

    // cleanup
#ifndef _WIN32
    close(fd);
#endif

    fs::remove_all(tempdir);
}
