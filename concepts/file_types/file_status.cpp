#include <iostream>
#include <set>
#include <string>
#include <algorithm>

#include "file_status.h"

void print_file_status(fs::path path)
{
    print_file_status(path.generic_string().c_str());
}

void print_file_status(const fs::directory_entry& entry)
{
    print_file_status(entry.path());
}

bool is_reserved(const char* path)
// https://learn.microsoft.com/en-gb/windows/win32/fileio/naming-a-file#naming-conventions
{

#ifdef _WIN32

    std::set<std::string> reserved {
      "CON", "PRN", "AUX", "NUL",
      "COM0", "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9",
      "LPT0", "LPT1", "LPT2", "LPT3", "LPT4", "LPT5", "LPT6", "LPT7", "LPT8", "LPT9"};

    auto s = std::string(path);
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);

#if __cplusplus >= 202002L
    return reserved.contains(s);
#else
    return reserved.find(s) != reserved.end();
#endif

#else
    return false;
#endif

}

void print_file_status(const char* path)
{
    fs::path p(path);

    std::cout << p;

    if(is_reserved(path)){
        // MSVC will crash
        std::cout << " is a reserved name\n";
        return;
    }

    auto s = fs::status(p);

    // alternative: switch(s.type()) { case fs::file_type::regular: ...}
    if(fs::is_regular_file(s)) std::cout << " is a regular file\n";
    if(fs::is_directory(s)) std::cout << " is a directory\n";
    if(fs::is_block_file(s)) std::cout << " is a block device\n";
    if(fs::is_character_file(s)) std::cout << " is a character device\n";
    if(fs::is_fifo(s)) std::cout << " is a named IPC pipe\n";
    if(fs::is_socket(s)) std::cout << " is a named IPC socket\n";
    if(fs::is_symlink(s)) std::cout << " is a symlink\n";
    if(!fs::exists(s)) std::cout << " does not exist\n";
}
