#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#ifdef _MSC_VER
#include <crtdbg.h>
#endif

#include "ffilesystem.h"

int main(void){

#ifdef _MSC_VER
  _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
  _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
#endif

#ifdef _WIN32
    char s[] = "NUL";
    const char ref[] = "NUL";
#else
    char s[] = "/dev/null";
    const char ref[] = "/dev/null";
#endif

    const size_t maxp = fs_get_max_path();

    char* p = (char*)malloc(maxp * sizeof(char));
    if(!p) goto err;

    printf("Begin test_reserved\n");

    fs_normal(s, p, maxp);
    if (strcmp(p, ref) != 0){
      fprintf(stderr,"FAIL: normal(%s)  %s\n", s, p);
      goto err;
    }
    printf("OK: normal(%s)\n", p);

    bool b = fs_is_absolute(s);
    if (fs_is_windows()){
      if(b) goto err;
    }
    else{
      if(!b) goto err;
    }
    printf("OK: is_absolute(%s)\n", ref);

    if(fs_is_dir(s)){
      fprintf(stderr,"FAIL: is_dir(%s)\n", s);
      goto err;
    }

    if(fs_is_exe(s)){
      fprintf(stderr,"FAIL: is_exe(%s)\n", s);
      goto err;
    }

if(!fs_is_windows()){

    // NOTE: do not test
    //
    // create_directories(/dev/null)
    // remove(/dev/null)
    // create_symlink()
    // set_permissionss()
    //
    // since if testing with "root" privilidges,
    // it can make the system unusable until reboot!

    if(!fs_exists(s)) goto err;
    printf("OK: exists(%s)\n", ref);

    b = fs_is_file(s);
    if(b){
      fprintf(stderr,"FAIL: is_file(%s) %d\n", s, b);
      goto err;
    }

    if(fs_canonical(s, false, p, maxp) == 0){
      fprintf(stderr,"FAIL: canonical(%s)  %s\n", s, p);
      goto err;
    }
    printf("OK: canonical(%s)\n", p);

    fs_relative_to(s, s, p, maxp);
    if(strcmp(p, ".") != 0){
      fprintf(stderr,"FAIL: relative_to(%s)  %s\n", ref, p);
      goto err;
    }
}

    fs_expanduser(s, p, maxp);
    if(strcmp(p, ref) != 0) goto err;

    if(fs_copy_file(s, s, false)){
      fprintf(stderr,"FAIL: copy_file(%s)\n", s);
      goto err;
    }
    printf("OK: copy_file(%s)\n", s);

    if(fs_touch(s)) goto err;
    printf("OK: touch(%s)\n", ref);

    if(fs_file_size(s) != 0) goto err;
    printf("OK: file_size(%s)\n", ref);

    if(fs_is_symlink(s)) goto err;
    printf("OK: is_symlink(%s)\n", s);

    printf("PASS: test_reserved.cpp\n");

    free(p);
    return EXIT_SUCCESS;

err:
    free(p);
    return EXIT_FAILURE;
}
