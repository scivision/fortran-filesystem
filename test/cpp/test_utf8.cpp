#include <iostream>
#include <string>
#include <exception>
#include <cstdlib>

#include "ffilesystem.h"

int main()
{

std::string smiley = "😀", wink = "😉", hello = "你好";

std::string u1;

// test allocation
u1 = fs_canonical(".", true);
std::cout << "canonical(.): " << u1 << "\n";

for (auto u : {smiley, wink, hello}) {
  u1 = fs_file_name("./" + u);
  if (u1 != u)
    throw std::runtime_error("fs_file_name(./" + smiley + ") != " + u1 + " " + u);


  u1 = fs_canonical(u, false);
  std::cout << "canonical(" + u + "): " << u1 << "\n";
  if (u1 != u)
    throw std::runtime_error("canonical UTF8: "  + u1 + " " + u);
}

return EXIT_SUCCESS;
}
