#include <stdio.h>
#include <stdlib.h>

#include "canonical.h"


#define MAXP 256

int main(int argc, char* argv[]){

  char out[MAXP];

  if(argc != 2){
    fprintf(stderr, "Usage: %s <path>\n", argv[0]);
    return EXIT_FAILURE;
  }

  if(!fs_realpath(argv[1], out, MAXP)){
    return EXIT_FAILURE;
  }

  printf("%s\n", out);

  return EXIT_SUCCESS;
}
