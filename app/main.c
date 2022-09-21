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
  else if (strcmp(argv[1], "is_linux") ==0){
    printf("%d\n", is_linux());
  }
  else if (strcmp(argv[1], "is_macos") ==0){
    printf("%d\n", is_macos());
  }
  else if (strcmp(argv[1], "is_unix") ==0){
    printf("%d\n", is_unix());
  }
  else if (strcmp(argv[1], "is_windows") ==0){
    printf("%d\n", is_windows());
  }
  else if (strcmp(argv[1], "file_size") ==0 && argc == 3){
    printf("%ld\n", file_size(argv[2]));
  }
  else if (strcmp(argv[1], "exists") ==0 && argc == 3){
    printf("%d\n", exists(argv[2]));
  }
  else if (strcmp(argv[1], "is_dir") ==0 && argc == 3){
    printf("%d\n", is_dir(argv[2]));
  }
  else if (strcmp(argv[1], "is_file") ==0 && argc == 3){
    printf("%d\n", is_file(argv[2]));
  }
  else if (strcmp(argv[1], "is_symlink") ==0 && argc == 3){
    printf("%d\n", is_symlink(argv[2]));
  }
  else{
    fprintf(stderr, "fs_cli <function_name> [<arg1> ...]");
    return EXIT_FAILURE;
  }


  return EXIT_SUCCESS;

}
