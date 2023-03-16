#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _MSC_VER
#include <io.h>
#else
#include <unistd.h>
#endif

#define MAXP 256

int main(int argc, char* argv[]){

  char out[MAXP];

  char* t;
#ifdef _WIN32
  t = _fullpath(out, argv[1], MAXP);
#else
  t = realpath(argv[1], out);
#endif

  if(!t){
    printf("Error: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  printf("canonical: %s => %s\n", argv[1], out);

  return EXIT_SUCCESS;

}
