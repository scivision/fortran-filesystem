#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <exception>

#include <filesystem>

#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN
#include <crtdbg.h>
#endif

#include "ffilesystem.h"


int main(){
#ifdef _MSC_VER
  _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
  _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
#endif

while (true){

  std::string inp;

  std::cout << "Ffilesystem> ";

  std::getline(std::cin, inp);

  // "\x04" is Ctrl-D on Windows.
  // EOF for non-Windows
  if (std::cin.eof() || inp == "\x04" || inp == "q")
    break;

  // split variable inp on space-delimiters
  const char delimiter = ' ';
  size_t pos = 0;
  std::vector<std::string> args;
  // FIXME: loop getline() instead
  while ((pos = inp.find(delimiter)) != std::string::npos) {
      args.push_back(inp.substr(0, pos));
      inp.erase(0, pos + 1);  // + 1 as delimiter is 1 char
  }
  // last argument
  args.push_back(inp);

  size_t argc = args.size();

  if (argc == 1){

  if (args.at(0) == "cpp")
    std::cout << fs_cpp() << "\n";
  else if (args.at(0) == "lang")
    std::cout << fs_lang() << "\n";
  else if (args.at(0) == "pathsep")
    std::cout << fs_pathsep() << "\n";
  else if (args.at(0) == "compiler")
    std::cout << fs_compiler() << "\n";
  else if (args.at(0) == "homedir")
    std::cout << fs_get_homedir() << "\n";
  else if (args.at(0) == "cwd")
    std::cout << fs_get_cwd() << "\n";
  else if (args.at(0) == "tempdir")
    std::cout << fs_get_tempdir() << "\n";
  else if (args.at(0) == "is_admin")
    std::cout << fs_is_admin() << "\n";
  else if (args.at(0) == "is_bsd")
    std::cout << fs_is_bsd() << "\n";
  else if (args.at(0) == "is_linux")
    std::cout << fs_is_linux() << "\n";
  else if (args.at(0) == "is_macos")
    std::cout << fs_is_macos() << "\n";
  else if (args.at(0) == "is_unix")
    std::cout << fs_is_unix() << "\n";
  else if (args.at(0) == "is_windows")
    std::cout << fs_is_windows() << "\n";
  else if (args.at(0) == "is_wsl")
    std::cout << fs_is_wsl() << "\n";
  else if (args.at(0) == "is_mingw")
    std::cout << fs_is_mingw() << "\n";
  else if (args.at(0) == "is_cygwin")
    std::cout << fs_is_cygwin() << "\n";
  else if (args.at(0) == "exe_dir")
    std::cout << fs_exe_dir() << "\n";
  else if (args.at(0) == "exe_path")
    std::cout << fs_exe_path() << "\n";
  else if (args.at(0) == "lib_path")
    std::cout << fs_lib_path() << "\n";
  else if (args.at(0) == "lib_dir")
    std::cout << fs_lib_dir() << "\n";
  else {
    std::cerr << args.at(0) << " requires more arguments or is unknown function\n";
  }

  continue;

  }

  // else if (inp == "touch"){
  //   std::cout << "touch " << inp << "\n";
  //   fs_touch(inp);
  // }
  // else if (inp == "remove") {
  //   std::cout << "remove " << inp << " " << fs_remove(inp) << "\n";
  // }

  // two argument functions
  if(argc == 2){

  if (args.at(0) == "expanduser")
    std::cout << fs_expanduser(args.at(1)) << "\n";
  else if (args.at(0) == "which")
    std::cout << fs_which(args.at(1)) << "\n";
  else if (args.at(0) == "canonical")
    std::cout << fs_canonical(args.at(1), false) << "\n";
  else if (args.at(0) == "resolve")
    std::cout << fs_resolve(args.at(1), false) << "\n";
  else if (args.at(0) == "parent")
    std::cout << fs_parent(args.at(1)) << "\n";
  else if (args.at(0) == "root")
    std::cout << fs_root(args.at(1)) << "\n";
  else if (args.at(0) == "stem")
    std::cout << fs_stem(args.at(1)) << "\n";
  else if (args.at(0) == "is_absolute")
    std::cout << fs_is_absolute(args.at(1)) << "\n";
  else if (args.at(0) == "exists")
    std::cout << fs_exists(args.at(1)) << "\n";
  else if (args.at(0) == "is_char")
    std::cout << fs_is_char_device(args.at(1)) << "\n";
  else if (args.at(0) == "is_dir")
    std::cout << fs_is_dir(args.at(1)) << "\n";
  else if (args.at(0) == "is_file")
    std::cout << fs_is_file(args.at(1)) << "\n";
  else if (args.at(0) == "is_exe")
    std::cout << fs_is_exe(args.at(1)) << "\n";
  else if (args.at(0) == "is_reserved")
    std::cout << fs_is_reserved(args.at(1)) << "\n";
  else if (args.at(0) == "is_readable")
    std::cout << fs_is_readable(args.at(1)) << "\n";
  else if (args.at(0) == "is_writable")
    std::cout << fs_is_writable(args.at(1)) << "\n";
  else if (args.at(0) == "perm")
    std::cout << fs_get_permissions(args.at(1)) << "\n";
  else if (args.at(0) == "long2short"){
    try {
      std::cout << fs_long2short(args.at(1)) << "\n";
    } catch (std::filesystem::filesystem_error& e){
      std::cerr << e.what() << "\n";
    }
    std::cout << fs_long2short(args.at(1)) << "\n";
  } else if (args.at(0) == "short2long"){
    try {
      std::cout << fs_short2long(args.at(1)) << "\n";
    } catch (std::filesystem::filesystem_error& e){
      std::cerr << e.what() << "\n";
    }
  } else if (args.at(0) == "is_symlink")
    std::cout << fs_is_symlink(args.at(1)) << "\n";
  else if (args.at(0) == "normal")
    std::cout << fs_normal(args.at(1)) << "\n";
  else if (args.at(0) == "size")
    try {
      std::cout << fs_file_size(args.at(1)) << "\n";
    } catch (std::filesystem::filesystem_error& e){
      std::cerr << e.what() << "\n";
    }
  else if (args.at(0) == "mkdir"){
    try {
      std::cout << "mkdir " << args.at(1) << "\n";
      fs_create_directories(args.at(1));
    } catch (std::filesystem::filesystem_error& e){
      std::cerr << e.what() << "\n";
    }
  }
  else if (args.at(0) == "chdir" || args.at(0) == "set_cwd") {
    std::cout << "cwd: " << fs_get_cwd() << "\n";
    try {
      fs_set_cwd(args.at(1));
      std::cout << "new cwd: " << fs_get_cwd() << "\n";
    } catch (std::filesystem::filesystem_error& e){
      std::cerr << e.what() << "\n";
    }
  } else {
    std::cerr << args.at(0) << " requires more arguments or is unknown function\n";
  }

  continue;

  }


  if (argc == 3){

  if (args.at(0) == "is_subdir")
    std::cout << fs_is_subdir(args.at(1), args.at(2)) << "\n";
  else if (args.at(0) == "relative_to")
    std::cout << fs_relative_to(args.at(1), args.at(2)) << "\n";
  else if (args.at(0) == "same")
    std::cout << fs_equivalent(args.at(1), args.at(2)) << "\n";
  else if (args.at(0) == "create_symlink"){
    std::cout << "create_symlink " << args.at(1) << " <= " << args.at(2) << "\n";
    try {
      fs_create_symlink(args.at(1), args.at(2));
    } catch (std::filesystem::filesystem_error& e){
      std::cerr << e.what() << "\n";
    }
  } else if (args.at(0) == "copy_file"){
    std::cout << "copy_file " << args.at(1) << " => " << args.at(2) << "\n";
    try {
      fs_copy_file(args.at(1), args.at(2), false);
    } catch (std::filesystem::filesystem_error& e){
      std::cerr << e.what() << "\n";
    }
  } else{
      std::cerr << args.at(0) << " requires more arguments or is unknown function\n";
    }

    continue;
  }

  if(argc == 5){

  if (args.at(0) == "set_perm"){
    int r = std::stoi(args.at(2));
    int w = std::stoi(args.at(3));
    int x = std::stoi(args.at(4));

    std::cout << "before chmod " << args.at(1) << " " << fs_get_permissions(args.at(1)) << "\n";

    try {
      fs_set_permissions(args.at(1), r, w, x);
    } catch (std::filesystem::filesystem_error& e){
      std::cerr << e.what() << "\n";
      continue;
    }

    std::cout << "after chmod " << args.at(1) << " " << fs_get_permissions(args.at(1)) << "\n";
  } else {
    std::cerr << args.at(0) << " requires more arguments or is unknown function\n";
  }


    continue;
  }

}

return EXIT_SUCCESS;

}
