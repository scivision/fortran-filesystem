#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <string>

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
  if(argc < 2 || argc > 3){
    std::cerr << "Usage: " << argv[0] << " <top dir> [-r]" << std::endl;
    return EXIT_FAILURE;
  }

  if (argc == 2 || (argc == 3 && std::string(argv[2]) != "-r")){
    std::cout << "std::filesystem::directory_iterator:" << std::endl;
    for (auto const& d : fs::directory_iterator{fs::path(argv[1])})
    {
      if (fs::is_directory(d))
      std::cout << d.path() << std::endl;
    }
  }
  else {
    std::cout << "\nstd::filesystem::recursive_directory_iterator:" << std::endl;
    for (auto const& d : fs::recursive_directory_iterator{fs::path(argv[1])})
    {
      if (fs::is_directory(d))
      std::cout << d.path() << std::endl;
    }
  }

  return EXIT_SUCCESS;
}
