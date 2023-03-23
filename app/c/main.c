#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef _MSC_VER
#include <crtdbg.h>
#endif

#include "ffilesystem.h"


int main(int argc, char* argv[]){
#ifdef _MSC_VER
  _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
  _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
#endif

  char p[MAXP];

  if (argc == 1) {
    fprintf(stderr, "fs_cli <function_name> [<arg1> ...]\n");
    return EXIT_FAILURE;
  }
  else if (strcmp(argv[1], "canonical") == 0 && argc == 3){
    if(fs_canonical(argv[2], false, p, MAXP))
      printf("%s\n", p);
  }
  else if (strcmp(argv[1], "compiler") == 0){
    fs_compiler(p, MAXP);
    printf("%s\n", p);
  }
  else if (strcmp(argv[1], "cpp") == 0){
    printf("%d\n", fs_cpp());
  }
  else if (strcmp(argv[1], "homedir") == 0) {
    if(fs_get_homedir(p, MAXP))
      printf("%s\n", p);
  }
  else if (strcmp(argv[1], "tempdir") == 0) {
    if(fs_get_tempdir(p, MAXP))
      printf("%s\n", p);
  }
  else if (strcmp(argv[1], "tempdir") == 0) {
    fs_get_tempdir(p, MAXP);
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
    printf("%d\n", fs_is_linux());
  }
  else if (strcmp(argv[1], "is_macos") ==0){
    printf("%d\n", fs_is_macos());
  }
  else if (strcmp(argv[1], "is_unix") == 0){
    printf("%d\n", fs_is_unix());
  }
  else if (strcmp(argv[1], "is_windows") == 0){
    printf("%d\n", fs_is_windows());
  }
  else if (strcmp(argv[1], "parent") == 0){
    if(fs_parent(argv[2], p, MAXP))
      printf("%s\n", p);
  }
  else if (strcmp(argv[1], "root") == 0 && argc == 3){
    if(fs_root(argv[2], p, MAXP))
      printf("%s\n", p);
  }
  else if (strcmp(argv[1], "file_size") ==0 && argc == 3){
    printf("%ju\n", fs_file_size(argv[2]));
  }
  else if (strcmp(argv[1], "exists") ==0 && argc == 3){
    printf("%d\n", fs_exists(argv[2]));
  }
  else if (strcmp(argv[1], "is_dir") ==0 && argc == 3){
    printf("%d\n", fs_is_dir(argv[2]));
  }
  else if (strcmp(argv[1], "is_exe") ==0 && argc == 3){
    printf("%d\n", fs_is_exe(argv[2]));
  }
  else if (strcmp(argv[1], "is_file") ==0 && argc == 3){
    printf("%d\n", fs_is_file(argv[2]));
  }
  else if (strcmp(argv[1], "is_symlink") ==0 && argc == 3){
    printf("%d\n", fs_is_symlink(argv[2]));
  }
  else if (strcmp(argv[1], "mkdir") == 0 && argc == 3){
    printf("mkdir %s\n", argv[2]);
    if(fs_create_directories(argv[2]) != 0)
      fprintf(stderr, "Failed mkdir %s\n", argv[2]);
  }
  else if (strcmp(argv[1], "relative_to") ==0 && argc == 4){
    if(fs_relative_to(argv[2], argv[3], p, MAXP))
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
