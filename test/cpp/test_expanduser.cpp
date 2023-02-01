#include <iostream>
#include <cstdlib>
#include <cstring>

#include "ffilesystem.h"

int main(void){

  char r[MAXP];
  char h[MAXP];

  size_t L = fs_expanduser("", r, MAXP);

  if(L != 0) {
    std::cerr << "expanduser('') != ''" << std::endl;
    return EXIT_FAILURE;
  }

  L = fs_expanduser(".", r, MAXP);
  if (L != 1 || std::strcmp(r, ".") != 0){
    std::cerr << "expanduser dot failed: " << r << std::endl;
    return EXIT_FAILURE;
  }

  L = fs_expanduser("~", r, MAXP);
  size_t L2 = fs_get_homedir(h, MAXP);
  if(L != L2 || std::strcmp(r, h) != 0){
    std::cerr << "expanduser home failed: " << r << h << std::endl;
    return EXIT_FAILURE;
  }

  L2 = fs_expanduser("~//", h, MAXP);
  if(L != L2 || std::strcmp(r, h) != 0){
    std::cerr << "expanduser double separator failed: " << r << h << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "OK: filesystem_C: expanduser: " << h << std::endl;

  return EXIT_SUCCESS;
}
