// from: https://wiki.sei.cmu.edu/confluence/pages/viewpage.action?pageId=87152177
// https://linux.die.net/man/3/execvp

#include <stddef.h>
#include <stdlib.h>

#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#endif

#include <stdio.h>

int main(void)
{
int r;
#ifdef _WIN32
  // don't directly specify "cmd.exe" in exec() for security reasons
  char* comspec = getenv("COMSPEC");
  if(!comspec){
    fprintf(stderr, "ERROR: environment variable COMSPEC not defined\n");
    return EXIT_FAILURE;
  }
  intptr_t ir = _execl(comspec, "cmd", "/c", "whoami",  NULL);
  r = (int)ir;
#else
  r = execlp("whoami", "whoami", NULL);
#endif

  if(r != -1)
    return EXIT_SUCCESS;

  return r;
}
