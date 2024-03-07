// no need to duplicate this in test/cpp
#include <exception>
#include <cstdlib>
#include <iostream>

#ifdef _MSC_VER
#include <crtdbg.h>
#endif

#include "ffilesystem.h"

int main(int argc, char *argv[]){

#ifdef _MSC_VER
  _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
  _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
#endif

  if(argc != 1){
    fprintf(stderr, "Usage: %s\n", argv[0]);
    return EXIT_FAILURE;
  }

  std::string drive(argv[0]);

  if(Ffs::file_size(drive) == 0)
    throw std::runtime_error("failed to get own file size");

  uintmax_t avail = Ffs::space_available(drive);
  if(avail == 0)
    throw std::runtime_error("failed to get space available of own drive " + drive);

  float avail_GB = (float) avail / 1073741824;
  std::cout << "OK space available on drive of " << drive <<  " (GB) " <<  avail_GB << "\n";

  return EXIT_SUCCESS;
}
