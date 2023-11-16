// no need to duplicate this in test/cpp
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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

    (void) argc;

    if(fs_file_size(argv[0]) == 0){
        fprintf(stderr, "failed to get own executable fs_file_size\n");
        return EXIT_FAILURE;
    }

    uintmax_t avail = fs_space_available(argv[0]);
    if(avail == 0){
        fprintf(stderr, "failed to get own drive fs_space_available  %s \n", argv[0]);
        return EXIT_FAILURE;
    }

    float avail_GB = (float) avail / 1073741824;
    printf("space available on drive of %s (GB) %f\n", argv[0], avail_GB);

  printf("OK: test_file\n");
  return EXIT_SUCCESS;
}
