#include <vector>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;


extern "C" size_t file_parts(const char* path, char* aparts[], size_t L[]) {

  std::vector <std::string> vparts;
  std::vector <size_t> Len;

  for (auto& p : fs::path(path)) {
    auto s = p.string();
    Len.push_back(s.length());
    vparts.push_back(s);
  }

  size_t N = vparts.size();

  aparts = new char*[N];

  for(size_t i = 0; i < N; i++){
      aparts[i] = new char[vparts[i].size() + 1];
      strcpy(aparts[i], vparts[i].c_str());
      L[i] = Len[i];
  }

  return N;
}
