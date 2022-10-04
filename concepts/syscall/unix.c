// from: https://wiki.sei.cmu.edu/confluence/pages/viewpage.action?pageId=87152177

#ifdef _WIN32
  char *const args[5] = {"cmd", "/c", "mkdir", p, NULL};
  int ret = execvp("cmd", args);
#else
  char *const args[4] = {"mkdir", "-p", p, NULL};
  int ret = execvp("mkdir", args);
#endif

  if(ret != -1)
    return 0;
