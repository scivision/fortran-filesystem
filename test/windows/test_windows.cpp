#include <cstdlib>
#include <iostream>
#include <exception>
#include <string>

#include "ffilesystem.h"

int main(int argc, char** argv){

    std::string long_path;

    if(argc < 2)
      long_path = std::getenv("PROGRAMFILES");
    else
      long_path = argv[1];

    if (long_path.empty())
      throw std::runtime_error("input is empty");

    std::string short_path = Ffs::shortname(long_path);

    std::cout << long_path << " => " << short_path << '\n';
    if(short_path.empty())
      throw std::runtime_error("short_path is empty");

    std::string long_path2 = Ffs::longname(short_path);

    std::cout << short_path << " => " << long_path2 << '\n';
    if(long_path2.empty())
      throw std::runtime_error("long_path is empty");

    if (long_path != long_path2)
      throw std::runtime_error("long_path != long_path2");

    return EXIT_SUCCESS;
}
