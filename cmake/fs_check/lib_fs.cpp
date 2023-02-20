#include <cstring>
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

void check_regex(char* s) {
  // some broken C++ platforms don't have std::regex linking properly
  std::string p(s);
  std::regex r("/{2,}");
  std::replace(p.begin(), p.end(), '\\', '/');
  p = std::regex_replace(p, r, "/");
  std::strcpy(s, p.c_str());
}
