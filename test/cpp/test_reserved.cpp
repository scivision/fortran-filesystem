#include <cstdlib>
#include <iostream>
#include <cstring>

#include <ffilesystem.h>



int main(){

#ifdef _WIN32
    char s[] = "NUL";
    const char ref[] = "NUL";
#else
    char s[] = "/dev/null";
    const char ref[] = "/dev/null";
#endif

    char p[MAXP];

    fs_normal(s, p, MAXP);
    if (std::strcmp(p, ref) != 0)
      return EXIT_FAILURE;

    if(fs_is_symlink(s))
      return EXIT_FAILURE;
    std::cout << "OK: is_symlink(" << ref << ")\n";

    if(fs_create_symlink(s, s) == 0)
      return EXIT_FAILURE;

    if(fs_create_directories(s) == 0)
      return EXIT_FAILURE;
    std::cout << "OK: create_directories(" << ref << ")\n";

    if(!fs_exists(s)){
      std::cerr << "FAIL: exists(" << ref << ")\n";
      return EXIT_FAILURE;
    }
    std::cout << "OK: exists(" << ref << ")\n";

    auto b = fs_is_absolute(s);
    if (fs_is_windows()){
      if(b) return EXIT_FAILURE;
    }
    else{
      if(!b) return EXIT_FAILURE;
    }
    std::cout << "OK: is_absolute(" << ref << ")\n";

    if(fs_is_dir(s))
      return EXIT_FAILURE;

    if(fs_is_exe(s))
      return EXIT_FAILURE;

    b = fs_is_file(s);
    if(b){
      std::cerr << "FAIL: is_file(" << s << ") " << b << "\n";
      return EXIT_FAILURE;
    }

    if(fs_remove(s)){
      std::cerr << "FAIL: remove(" << s << ")\n";
      return EXIT_FAILURE;
    }
    std::cout << "OK: remove(" << s << ")\n";

    if(fs_canonical(s, false, p, MAXP) == 0)
      return EXIT_FAILURE;
    std::cout << "OK: canonical(" << p << ")\n";

    if(fs_equivalent(s, s)){
    // reserved we treat like NaN not equal
      std::cerr << "FAIL: equivalent(" << s << ")\n";
      return EXIT_FAILURE;
    }
    std::cout << "OK: equivalent(" << s << ")\n";

    fs_expanduser(s, p, MAXP);
    if(std::strcmp(p, ref) != 0)
      return EXIT_FAILURE;

    if(fs_copy_file(s, s, false) == 0){
      std::cerr << "FAIL: copy_file(" << s << ")\n";
      return EXIT_FAILURE;
    }

    fs_relative_to(s, s, p, MAXP);
    if(std::strcmp(p, ".") != 0){
      std::cerr << "FAIL: relative_to(" << ref << "," << ref << ") " << p << "\n";
      return EXIT_FAILURE;
    }

    if(fs_touch(s))
      return EXIT_FAILURE;
    std::cout << "OK: touch(" << ref << ")\n";

    if(fs_file_size(s) != 0)
      return EXIT_FAILURE;

    if(fs_chmod_exe(s))
      return EXIT_FAILURE;

    if(fs_chmod_no_exe(s))
      return EXIT_FAILURE;

    std::cout << "PASS: test_reserved.cpp\n";

    return EXIT_SUCCESS;
}
