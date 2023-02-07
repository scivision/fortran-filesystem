// from: https://wiki.sei.cmu.edu/confluence/pages/viewpage.action?pageId=87152177
// https://linux.die.net/man/3/execvp

#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#endif

int main(void)
{
int r;
#ifdef _WIN32
  intptr_t ir = _execlp("cmd", "cmd", "/c", "dir",  NULL);
  r = (int)ir;
#else
  r = execlp("ls", "ls", ".", NULL);
#endif

  if(r != -1)
    return 0;

  return r;
}
