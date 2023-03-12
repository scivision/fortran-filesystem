#include <iostream>
#include <filesystem>
#include <cstdint>
#include <cstdlib>

namespace fs = std::filesystem;


uintmax_t space_avail(const char* path)
{
  if(!path)
    return 0;

  std::error_code ec;

  auto si = fs::space(path, ec);
  if(ec){
    std::cerr << "ERROR:ffilesystem:space_avail " << ec.message() << std::endl;
    return 0;
  }

  return static_cast<std::intmax_t>(si.available);
}

int main(int argc, char* argv[])
{
    if(argc<2){
      std::cerr << "Usage: cpp_space_avail <path>\n";
      return EXIT_FAILURE;
    }

    auto favail_GB = float(space_avail(argv[1])) / 1073741824;

    std::cout << "space_avail (GB): " << argv[1] << " " << favail_GB << std::endl;

    return EXIT_SUCCESS;
}
