// Check that filesystem is capable of symbolic links with this compiler.
// note: fs::status(lnk) with is_symlink is bugged on Windows--use fs::is_symlink(lnk) on path instead
#include <iostream>
#include <cstdlib>
#include <filesystem>

#ifndef __cpp_lib_filesystem
#error "No C++ filesystem support"
#endif

namespace fs = std::filesystem;


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

if(!fs::exists(lnk)) {
  std::cerr << "symlink not created: " << lnk << std::endl;
  return EXIT_FAILURE;
}

if(fs::is_symlink(lnk)) {
  std::cout << lnk << " is a symlink" << std::endl;
  return EXIT_SUCCESS;
}

std::cerr << "ERROR: " << lnk << " is not a symlink" << std::endl;

return EXIT_FAILURE;
}
