#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "myfs.h"


int main(int argc, char* argv[]) {

  bool has;

  if (argc < 2){
    has = has_filename(".");
  }
  else {
    has = has_filename(argv[1]);
  }

  printf("%d\n", has);

  return EXIT_SUCCESS;
}
