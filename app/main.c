#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ffilesystem.h"


int main(int argc, char* argv[]){

  char p[MAXP];

  if (argc == 1) {
      fprintf(stderr, "fs_cli <function_name> [<arg1> ...]");
      return EXIT_FAILURE;
  }
  else if (strcmp(argv[1], "homedir") == 0) {
    if(get_homedir(p, MAXP))
      printf("%s\n", p);
  }
  else if (strcmp(argv[1], "tempdir") == 0) {
    if(get_tempdir(p, MAXP))
      printf("%s\n", p);
  }
  else if (strcmp(argv[1], "tempdir") == 0) {
    get_tempdir(p, MAXP);
    printf("%s\n", p);
  }
  else if (strcmp(argv[1], "lib_path") == 0){
    if(fs_lib_path(p, MAXP))
      printf("%s\n", p);
  }
  else if (strcmp(argv[1], "exe_path") == 0){
    if(fs_exe_path(p, MAXP))
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
    printf("%ju\n", file_size(argv[2]));
  }
  else if (strcmp(argv[1], "exists") ==0 && argc == 3){
    printf("%d\n", exists(argv[2]));
  }
  else if (strcmp(argv[1], "is_dir") ==0 && argc == 3){
    printf("%d\n", is_dir(argv[2]));
  }
  else if (strcmp(argv[1], "is_exe") ==0 && argc == 3){
    printf("%d\n", is_exe(argv[2]));
  }
  else if (strcmp(argv[1], "is_file") ==0 && argc == 3){
    printf("%d\n", is_file(argv[2]));
  }
  else if (strcmp(argv[1], "is_symlink") ==0 && argc == 3){
    printf("%d\n", is_symlink(argv[2]));
  }
  else if (strcmp(argv[1], "relative_to") ==0 && argc == 4){
    if(relative_to(argv[2], argv[3], p, MAXP))
      printf("%s\n", p);
  }
  else if (strcmp(argv[1], "normal") ==0 && argc==3){
    if(fs_normal(argv[2], p, MAXP))
      printf("%s\n", p);
  }
  else{
    fprintf(stderr, "fs_cli <function_name> [<arg1> ...]");
    return EXIT_FAILURE;
  }


  return EXIT_SUCCESS;

}
