#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <ffilesystem.h>



int main(void){

#ifdef _WIN32
    char s[] = "NUL";
    const char ref[] = "NUL";
#else
    char s[] = "/dev/null";
    const char ref[] = "/dev/null";
#endif

    char p[MAXP];

    fs_normal(s, p, MAXP);
    if (strcmp(p, ref) != 0)
      return EXIT_FAILURE;

    if(fs_is_symlink(s))
      return EXIT_FAILURE;
    printf("OK: is_symlink(%s)\n", ref);

    if(fs_create_symlink(s, s) == 0)
      return EXIT_FAILURE;

    if(fs_create_directories(s) == 0)
      return EXIT_FAILURE;
    printf("OK: create_directories(%s)\n", ref);

    if(!fs_exists(s))
      return EXIT_FAILURE;
    printf("OK: exists(%s)\n", ref);

    bool b = fs_is_absolute(s);
    if (fs_is_windows()){
      if(b) return EXIT_FAILURE;
    }
    else{
      if(!b) return EXIT_FAILURE;
    }
    printf("OK: is_absolute(%s)\n", ref);

    if(fs_is_dir(s))
      return EXIT_FAILURE;

    if(fs_is_exe(s))
      return EXIT_FAILURE;

    b = fs_is_file(s);
    if(b){
      fprintf(stderr,"FAIL: is_file(%s) %d\n", s, b);
      return EXIT_FAILURE;
    }

    if(fs_remove(s)){
      fprintf(stderr,"FAIL: remove(%s)\n", s);
      return EXIT_FAILURE;
    }
    printf("OK: remove(%s)\n", s);

    if(fs_canonical(s, false, p, MAXP) == 0){
      fprintf(stderr,"FAIL: canonical(%s)  %s\n", s, p);
      return EXIT_FAILURE;
    }
    printf("OK: canonical(%s)\n", p);

    if(fs_equivalent(s, s))
      return EXIT_FAILURE;
    printf("OK: equivalent(%s)\n", ref);

    fs_expanduser(s, p, MAXP);
    if(strcmp(p, ref) != 0)
      return EXIT_FAILURE;

    if(fs_copy_file(s, s, false) == 0){
      fprintf(stderr,"FAIL: copy_file(%s)\n", s);
      return EXIT_FAILURE;
    }

    fs_relative_to(s, s, p, MAXP);
    if(strcmp(p, ".") != 0){
      fprintf(stderr,"FAIL: relative_to(%s)  %s\n", ref, p);
      return EXIT_FAILURE;
    }

    if(fs_touch(s))
      return EXIT_FAILURE;
    printf("OK: touch(%s)\n", ref);

    if(fs_file_size(s) != 0)
      return EXIT_FAILURE;

    if(fs_chmod_exe(s))
      return EXIT_FAILURE;

    if(fs_chmod_no_exe(s))
      return EXIT_FAILURE;

    printf("PASS: test_reserved.cpp\n");

    return EXIT_SUCCESS;
}
