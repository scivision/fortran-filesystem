// from: https://wiki.sei.cmu.edu/confluence/pages/viewpage.action?pageId=87152177
// https://linux.die.net/man/3/execvp

#include <stddef.h>
#include <unistd.h>

int main(void){

#ifdef _WIN32
  char *const args[4] = {"cmd", "/c", "dir", NULL};
#else
  char *const args[3] = {"ls", ".", NULL};
#endif

  int ret = execvp(args[0], args);

  if(ret != -1)
    return 0;

  return ret;

}
