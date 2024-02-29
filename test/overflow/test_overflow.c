#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ffilesystem.h"

int main(void){

  size_t N = 5;
  char* buf = (char*) malloc(N);
  int i=0;

  if(fs_normal("abcedf", buf, N) != 0){
    fprintf(stderr, "ERROR: fs_normal(abcdef) did not handle overflow properly\n");
    fprintf(stderr, "       buf = %s\n", buf);
    i++;
  }

  if(fs_expanduser("~", buf, N) != 0){
    fprintf(stderr, "ERROR: fs_expanduser(~) did not handle overflow properly\n");
    fprintf(stderr, "       buf = %s\n", buf);
    i++;
  }

  if(fs_expanduser("abcedf", buf, N) != 0){
    fprintf(stderr, "ERROR: fs_expanduser(abcdef) did not handle overflow properly\n");
    fprintf(stderr, "       buf = %s\n", buf);
    i++;
  }

  if(fs_file_name("abcedf", buf, N) != 0){
    fprintf(stderr, "ERROR: fs_file_name(abcdef) did not handle overflow properly\n");
    fprintf(stderr, "       buf = %s\n", buf);
    i++;
  }

  if(fs_stem("abcedf", buf, N) != 0){
    fprintf(stderr, "ERROR: fs_stem(abcdef) did not handle overflow properly\n");
    fprintf(stderr, "       buf = %s\n", buf);
    i++;
  }

  if(fs_parent("abcedf", buf, N) != 0){
    fprintf(stderr, "ERROR: fs_parent(abcdef) did not handle overflow properly\n");
    fprintf(stderr, "       buf = %s\n", buf);
    i++;
  }

  if(fs_with_suffix(".abcedf", "txt", buf, N) != 0){
    fprintf(stderr, "ERROR: fs_with_suffix(abcdef, txt) did not handle overflow properly\n");
    fprintf(stderr, "       buf = %s\n", buf);
    i++;
  }

  if(fs_root("abcdef", buf, N) != 0){
    fprintf(stderr, "ERROR: fs_root(abcdef) did not handle overflow properly\n");
    fprintf(stderr, "       buf = %s\n", buf);
    i++;
  }

  if(fs_make_absolute("abcdef", "zyxwvu", buf, N) != 0){
    fprintf(stderr, "ERROR: fs_make_absolute(abcdef) did not handle overflow properly\n");
    fprintf(stderr, "       buf = %s\n", buf);
    i++;
  }

  if(fs_short2long("abcdef", buf, N) != 0){
    fprintf(stderr, "ERROR: fs_short2long(abcdef) did not handle overflow properly\n");
    fprintf(stderr, "       buf = %s\n", buf);
    i++;
  }

  if(fs_long2short("abcdef", buf, N) != 0){
    fprintf(stderr, "ERROR: fs_long2short(abcdef) did not handle overflow properly\n");
    fprintf(stderr, "       buf = %s\n", buf);
    i++;
  }

  free(buf);

  if(i > 0)
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
