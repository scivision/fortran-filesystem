#include <regex>

#if __has_include(<filesystem>)
#include <filesystem>
namespace fs = std::filesystem;
#else
#error "No C++ filesystem support"
#endif

extern "C" bool has_filename(const char*);

bool has_filename(const char* path){
  fs::path p(path);
  return p.has_filename();
}

void dummy() { std::regex r("/{2,}"); }
