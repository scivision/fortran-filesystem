#include <iostream>
#include <cstdlib>
#include <string>

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

  if (argc == 1) {
    std::cerr << "fs_cli <function_name> [<arg1> ...]\n";
    return EXIT_FAILURE;
  }

  std::string arg1 = argv[1];
  std::string arg2, arg3, arg4;
  if(argc > 2)
    arg2 = argv[2];
  if(argc > 3)
    arg3 = argv[3];
  if(argc > 4)
    arg4 = argv[4];

  if (arg1 == "canonical" && argc == 3){
    std::cout << fs_canonical(arg2, false) << "\n";
  }
  else if (arg1 == "compiler"){
    std::cout << fs_compiler() << "\n";
  }
  else if (arg1 == "cpp"){
    std::cout << fs_cpp() << "\n";
  }
  else if (arg1 == "homedir") {
    std::cout << fs_get_homedir() << "\n";
  }
  else if (arg1 == "tempdir") {
    std::cout << fs_get_tempdir() << "\n";
  }
  else if (arg1 == "tempdir") {
    std::cout << fs_get_tempdir() << "\n";
  }
  else if (arg1 == "lib_path"){
    std::cout << fs_lib_path() << "\n";
  }
  else if (arg1 == "lib_dir"){
    std::cout << fs_lib_dir() << "\n";
  }
  else if (arg1 == "exe_path"){
    std::cout << fs_exe_path() << "\n";
  }
  else if (arg1 == "exe_dir"){
    std::cout << fs_exe_dir() << "\n";
  }
  else if (arg1 == "is_linux"){
    std::cout << fs_is_linux() << "\n";
  }
  else if (arg1 == "is_macos"){
    std::cout << fs_is_macos() << "\n";
  }
  else if (arg1 == "is_unix"){
    std::cout << fs_is_unix() << "\n";
  }
  else if (arg1 == "is_windows"){
    std::cout << fs_is_windows() << "\n";
  }
  else if (arg1 == "parent"){
    std::cout << fs_parent(arg2) << "\n";
  }
  else if (arg1 == "root" && argc == 3){
    std::cout << fs_root(arg2) << "\n";
  }
  else if (arg1 == "file_size" && argc == 3){
    std::cout << fs_file_size(arg2) << "\n";
  }
  else if (arg1 == "exists" && argc == 3){
    std::cout << fs_exists(arg2) << "\n";
  }
  else if (arg1 == "is_dir" && argc == 3){
    std::cout << fs_is_dir(arg2) << "\n";
  }
  else if (arg1 == "is_exe" && argc == 3){
    std::cout << fs_is_exe(arg2) << "\n";
  }
  else if (arg1 == "is_file" && argc == 3){
    std::cout << fs_is_file(arg2) << "\n";
  }
  else if (arg1 == "is_symlink" && argc == 3){
    std::cout << fs_is_symlink(arg2) << "\n";
  }
  else if (arg1 == "mkdir" && argc == 3){
    std::cout << "mkdir " << arg2 << "\n";
    if(fs_create_directories(arg2) != 0)
      std::cerr << "Failed mkdir " << arg2 << "\n";
  }
  else if (arg1 == "relative_to" && argc == 4){
    std::cout << fs_relative_to(arg2, arg3) << "\n";
  }
  else if (arg1 == "normal" && argc == 3){
    std::cout << fs_normal(arg2) << "\n";
  }
  else{
    std::cerr << "fs_cli <function_name> [<arg1> ...]" << "\n";
    return EXIT_FAILURE;
  }


  return EXIT_SUCCESS;

}
