#include <cstring>
#include <regex>
#include <filesystem>

static_assert(__cpp_lib_filesystem, "No C++ filesystem support");

namespace fs = std::filesystem;


extern "C" bool has_filename(const char*);

bool has_filename(const char* path){
  fs::path p(path);
  return p.has_filename();
}
