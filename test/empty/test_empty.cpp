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

    Ffs::as_posix(O);
    std::cout << "PASS: as_posix(char*)\n";

    // p = Ffs::as_posix("");
    // std::cout << "PASS: as_posix(string)\n";

    Ffs::as_windows(O);
    std::cout << "PASS: as_windows\n";

    if(!Ffs::normal("").empty())
      throw std::runtime_error("Ffs::normal");

    if(!Ffs::file_name("").empty())
      throw std::runtime_error("Ffs::file_name");

    if(!Ffs::stem("").empty())
      throw std::runtime_error("Ffs::stem");

    if(!Ffs::join("", "").empty())
      throw std::runtime_error("Ffs::join");

    if(!Ffs::parent("").empty())
      throw std::runtime_error("Ffs::parent");

    if(!Ffs::suffix("").empty())
      throw std::runtime_error("Ffs::suffix");

    if(!Ffs::with_suffix("", "").empty())
      throw std::runtime_error("Ffs::with_suffix");

    if(Ffs::is_char_device(""))
      throw std::runtime_error("Ffs::is_char_device");

    if(Ffs::is_reserved(""))
      throw std::runtime_error("Ffs::is_reserved");

    if(Ffs::is_symlink(""))
      throw std::runtime_error("Ffs::is_symlink");

    try{
      Ffs::create_symlink("", "");
    } catch (std::filesystem::filesystem_error& e){
      std::cerr << e.what() << "\n";
    }

    try{
      Ffs::mkdir("");
    } catch (std::filesystem::filesystem_error& e){
      std::cerr << e.what() << "\n";
    }

    if(!Ffs::root("").empty())
      throw std::runtime_error("Ffs::root");

    if(Ffs::exists(""))
      throw std::runtime_error("Ffs::exists");

    if(Ffs::is_absolute(""))
      throw std::runtime_error("Ffs::is_absolute");

    if(Ffs::is_dir(""))
      throw std::runtime_error("Ffs::is_dir");

    if(Ffs::is_exe(""))
      throw std::runtime_error("Ffs::is_exe");

    if(Ffs::is_file(""))
      throw std::runtime_error("Ffs::is_file");

    if(Ffs::remove(""))
      throw std::runtime_error("Ffs::remove");

    if(!Ffs::canonical("", false).empty())
      throw std::runtime_error("Ffs::canonical");

    if(Ffs::equivalent("", ""))
      throw std::runtime_error("Ffs::equivalent");

    if(!Ffs::expanduser("").empty())
      throw std::runtime_error("Ffs::expanduser");

    try{
      Ffs::copy_file("", "", false);
    } catch (std::filesystem::filesystem_error& e){
      std::cerr << e.what() << "\n";
    }

    if(!Ffs::relative_to("", "").empty())
      throw std::runtime_error("Ffs::relative_to");

    try{
      Ffs::touch("");
    } catch (std::filesystem::filesystem_error& e){
      std::cerr << e.what() << "\n";
    }

    try{
      if(Ffs::file_size("") != 0)
        throw std::runtime_error("Ffs::file_size");
    } catch (std::filesystem::filesystem_error& e){
      std::cerr << e.what() << "\n";
    }


    if(!fs_is_windows()) {
      try{
        if(Ffs::space_available("") != 0)
          throw std::runtime_error("Ffs::space_available");
      } catch (std::filesystem::filesystem_error& e){
        std::cerr << e.what() << "\n";
      }
    }

    if(Ffs::get_cwd().empty())
      throw std::runtime_error("get_cwd");

    if(Ffs::get_homedir().empty())
      throw std::runtime_error("get_homedir");

    if(fs_cpp()){
      if(!fs_is_bsd() && Ffs::exe_dir().empty())
        throw std::runtime_error("Ffs::exe_dir");

      bool le = Ffs::lib_dir().empty();
      if(shared){
        if (le) throw std::runtime_error("Ffs::lib_dir");
      } else {
        if (!le) throw std::runtime_error("Ffs::lib_dir");
      }
    }

    try{
      Ffs::set_permissions("", 0, 0, 0);
    } catch (std::filesystem::filesystem_error& e){
      std::cerr << e.what() << "\n";
    }

    std::cout << "OK: test_c_empty\n";

    return EXIT_SUCCESS;
}
