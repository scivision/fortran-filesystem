#include <iostream>
#include <string>
#include <cstdlib>

#include "ffilesystem.h"

[[noreturn]] void err(std::string_view m){
    std::cerr << "ERROR: " << m << "\n";
    std::exit(EXIT_FAILURE);
}

int main()
{

std::string smiley = "😀";
std::string wink = "😉";
std::string hello = "你好";

std::string u1;

// test allocation
u1 = Ffs::canonical(".", true);
std::cout << "canonical(.): " << u1 << "\n";

for ( const auto &u : {smiley, wink, hello} ) {
  u1 = Ffs::file_name("./" + u);
  if (u1 != u)
    err("Ffs::file_name(./" + smiley + ") != " + u1 + " " + u);


  u1 = Ffs::canonical(u, false);
  std::cout << "canonical(" + u + "): " << u1 << "\n";
  if (u1 != u)
    err("canonical UTF8: "  + u1 + " " + u);
}

return EXIT_SUCCESS;
}
