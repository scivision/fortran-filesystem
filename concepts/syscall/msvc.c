#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#define TRACE 0

int msvc_call(const char* path){

  char* p = (char*) malloc(strlen(path) + 1);
  strcpy(p, path);

  STARTUPINFO si = { 0 };
  PROCESS_INFORMATION pi;
  si.cb = sizeof(si);

  char* cmd = (char*) malloc(strlen(p) + 1 + 13);
  strcpy(cmd, "cmd /c dir ");
  strcat(cmd, p);
  free(p);

if(TRACE) printf("TRACE: %s\n", cmd);

  if (!CreateProcess(NULL, cmd, NULL, NULL, FALSE, 0, 0, 0, &si, &pi))
    return -1;

if(TRACE) printf("TRACE: waiting to complete %s\n", cmd);
  // Wait until child process exits.
  WaitForSingleObject( pi.hProcess, 2000 );
  CloseHandle(pi.hThread);
  CloseHandle(pi.hProcess);
if(TRACE) printf("TRACE: completed %s\n", cmd);

  return 0;
}

int main(void){
  return msvc_call("C:\\Users");
}
