#include <iostream>

#ifndef __has_include
#error "Compiler not C++17 compliant"
#endif

#if __has_include(<filesystem>)
#include <filesystem>
namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#error "No C++17 filesystem support"
#endif



int main(int argc, char **argv){

auto tgt = fs::canonical(argv[0]);
auto lnk = tgt.parent_path() / "test.lnk";

if(!fs::is_regular_file(tgt)) {
  std::cerr << "ERROR: target " << tgt << " is not a regular file" << std::endl;
  return EXIT_FAILURE;
}

if(!fs::exists(lnk)) {
  fs::create_symlink(tgt, lnk);
  std::cout << "created symlink: " << lnk << std::endl;
}

if(fs::exists(lnk) & fs::is_symlink(lnk)) {
  std::cout << lnk << " is a symlink" << std::endl;
  return EXIT_SUCCESS;
}

std::cerr << "ERROR: " << lnk << " is not a symlink" << std::endl;

return EXIT_FAILURE;
}
