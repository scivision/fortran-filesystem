#include <iostream>
#include <filesystem>

#ifndef __cpp_lib_filesystem
#error "Compiler doesn't support C++17 filesystem"
#endif

namespace fs = std::filesystem;

int main(int argc, char **argv){

auto tgt = fs::weakly_canonical(argv[0]);
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
