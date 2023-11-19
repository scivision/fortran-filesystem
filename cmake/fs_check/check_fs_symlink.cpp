// Check that filesystem is capable of symbolic links with this compiler.
#include <iostream>
#include <cstdlib>
#include <exception>

#include <filesystem>

static_assert(__cpp_lib_filesystem, "No C++ filesystem support");

namespace fs = std::filesystem;


int main(int argc, char **argv){

auto tgt = fs::canonical(argv[0]);
auto s = fs::status(tgt);

auto lnk = tgt.parent_path() / "test.lnk";

if(!fs::is_regular_file(s))
  throw std::runtime_error("target " + tgt.generic_string() + " is not a regular file");

if(!fs::exists(lnk)) {
  fs::create_symlink(tgt, lnk);
  std::cout << "created symlink: " << lnk << "\n";
}
auto l = fs::symlink_status(lnk);

if(!fs::exists(l))
  throw std::runtime_error("symlink not created: " + lnk.generic_string());

if(!fs::is_symlink(l))
  throw std::runtime_error("is not a symlink: " + lnk.generic_string());

return EXIT_SUCCESS;
}
