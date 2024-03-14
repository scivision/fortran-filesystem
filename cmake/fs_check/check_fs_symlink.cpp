// Check that filesystem is capable of symbolic links with this compiler.
#include <iostream>
#include <cstdlib>

#include <filesystem>

static_assert(__cpp_lib_filesystem, "No C++ filesystem support");

namespace fs = std::filesystem;

[[noreturn]] void err(std::string_view m){
    std::cerr << "ERROR: " << m << "\n";
    std::exit(EXIT_FAILURE);
}


int main(int argc, char **argv){

if(argc < 2)
  err("missing argument for target");

auto tgt = fs::canonical(argv[1]);

if(!fs::is_regular_file(tgt))
  err("target " + tgt.generic_string() + " is not a regular file");

auto lnk = tgt.parent_path() / "test.lnk";

if(!fs::exists(lnk)) {
  fs::create_symlink(tgt, lnk);
  std::cout << "created symlink: " << lnk << "\n";
}
auto l = fs::symlink_status(lnk);

if(!fs::exists(l))
  err("symlink not created: " + lnk.generic_string());

if(!fs::is_symlink(l))
  err("is not a symlink: " + lnk.generic_string());

return EXIT_SUCCESS;
}
