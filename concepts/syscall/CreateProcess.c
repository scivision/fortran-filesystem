// https://learn.microsoft.com/en-us/windows/win32/procthread/creating-processes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#ifndef TRACE
#define TRACE 1
#endif

int msvc_call(const char* path){

  char* p = (char*) malloc(strlen(path) + 1);
  strcpy(p, path);

  STARTUPINFO si = { 0 };
  PROCESS_INFORMATION pi;

  ZeroMemory( &si, sizeof(si) );
  si.cb = sizeof(si);
  ZeroMemory( &pi, sizeof(pi) );

  char* cmd = (char*) malloc(strlen(p) + 1 + 13);
  strcpy(cmd, "cmd /c dir ");
  strcat(cmd, p);
  free(p);

if(TRACE) printf("TRACE: %s\n", cmd);

  if (!CreateProcess(NULL, //  No module name (use command line)
    cmd,    // Command line
    NULL,   // Process handle not inheritable
    NULL,   // Thread handle not inheritable
    FALSE,  // Set handle inheritance to FALSE
    0,      // No creation flags
    NULL,   // Use parent's environment block
    NULL,   // Use parent's starting directory
    &si,    // Pointer to STARTUPINFO structure
    &pi )   // Pointer to PROCESS_INFORMATION structure
    )
    return EXIT_FAILURE;

if(TRACE) printf("TRACE: waiting to complete %s\n", cmd);
  // Wait until child process exits.
  WaitForSingleObject( pi.hProcess, 5000 );
  if (!CloseHandle(pi.hThread) || !CloseHandle(pi.hProcess))
    return EXIT_FAILURE;
if(TRACE) printf("TRACE: completed %s\n", cmd);

  return EXIT_SUCCESS;
}

int main(void){

  char* buf;
  const size_t Lb=2048;
  size_t L;

  buf = (char*) malloc(Lb);

  if(getenv_s(&L, buf, Lb, "USERPROFILE") != 0){
    fprintf(stderr, "ERROR: getenv_s failed\n");
    return EXIT_FAILURE;
  }

  int s = msvc_call(buf);

  free(buf);

  return s;
}
