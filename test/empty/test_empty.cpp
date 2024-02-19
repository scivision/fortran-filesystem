// verify functions handle empty input OK

#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>

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

    bool shared = false;
    if(argc > 1)
      shared = atoi(argv[1]);

    char O[1];

    O[0] = '\0';

    fs_as_posix(O);
    std::cout << "PASS: as_posix(char*)\n";

    // p = fs_as_posix("");
    // std::cout << "PASS: as_posix(string)\n";

    fs_as_windows(O);
    std::cout << "PASS: as_windows\n";

    if(!fs_normal("").empty())
      throw std::runtime_error("fs_normal");

    if(!fs_file_name("").empty())
      throw std::runtime_error("fs_file_name");

    if(!fs_stem("").empty())
      throw std::runtime_error("fs_stem");

    if(!fs_join("", "").empty())
      throw std::runtime_error("fs_join");

    if(!fs_parent("").empty())
      throw std::runtime_error("fs_parent");

    if(!fs_suffix("").empty())
      throw std::runtime_error("fs_suffix");

    if(!fs_with_suffix("", "").empty())
      throw std::runtime_error("fs_with_suffix");

    if(fs_is_char_device(""))
      throw std::runtime_error("fs_is_char_device");

    if(fs_is_reserved(""))
      throw std::runtime_error("fs_is_reserved");

    if(fs_is_symlink(""))
      throw std::runtime_error("fs_is_symlink");

    if(fs_create_symlink("", ""))
      throw std::runtime_error("fs_create_symlink");

    if(fs_create_directories(""))
      throw std::runtime_error("fs_create_directories");

    if(!fs_root("").empty())
      throw std::runtime_error("fs_root");

    if(fs_exists(""))
      throw std::runtime_error("fs_exists");

    if(fs_is_absolute(""))
      throw std::runtime_error("fs_is_absolute");

    if(fs_is_dir(""))
      throw std::runtime_error("fs_is_dir");

    if(fs_is_exe(""))
      throw std::runtime_error("fs_is_exe");

    if(fs_is_file(""))
      throw std::runtime_error("fs_is_file");

    if(fs_remove(""))
      throw std::runtime_error("fs_remove");

    if(!fs_canonical("", false).empty())
      throw std::runtime_error("fs_canonical");

    if(fs_equivalent("", ""))
      throw std::runtime_error("fs_equivalent");

    if(!fs_expanduser("").empty())
      throw std::runtime_error("fs_expanduser");

    if(fs_copy_file("", "", false))
      throw std::runtime_error("fs_copy_file");

    if(!fs_relative_to("", "").empty())
      throw std::runtime_error("fs_relative_to");

    if(fs_touch(""))
      throw std::runtime_error("fs_touch");

    if(fs_file_size("") != 0)
      throw std::runtime_error("fs_file_size");

    if(!fs_is_windows()) {
      if(fs_space_available("") != 0)
        throw std::runtime_error("fs_space_available");
    }

    if(fs_get_cwd().empty())
      throw std::runtime_error("fs_get_cwd");

    if(fs_get_homedir().empty())
      throw std::runtime_error("fs_get_homedir");

    if(fs_cpp()){
      if(!fs_is_bsd() && fs_exe_dir().empty())
        throw std::runtime_error("fs_exe_dir");

      bool le = fs_lib_dir().empty();
      if(shared){
        if (le) throw std::runtime_error("fs_lib_dir");
      } else {
        if (!le) throw std::runtime_error("fs_lib_dir");
      }
    }

    if(fs_set_permissions("", 0, 0, 0))
      throw std::runtime_error("fs_set_permissions");

    std::cout << "OK: test_c_empty\n";

    return EXIT_SUCCESS;
}
