#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "filesystem.h"


int main(int argc, char* argv[]){

  char p[MAXP];

  if (argc == 1) {
      fprintf(stderr, "fs_cli <function_name> [<arg1> ...]");
      return EXIT_FAILURE;
  }
  else if (strcmp(argv[1], "homedir") == 0) {
    get_homedir(p);
    printf("%s\n", p);
  }
  else if (strcmp(argv[1], "lib_path") == 0){
    lib_path(p);
    printf("%s\n", p);
  }
  else if (strcmp(argv[1], "is_windows") ==0){
    printf("%d\n", is_windows());
  }
  else{
    fprintf(stderr, "fs_cli <function_name> [<arg1> ...]");
    return EXIT_FAILURE;
  }


  return EXIT_SUCCESS;

}
