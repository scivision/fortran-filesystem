#include <stdbool.h>

#if defined(__unix__) || !defined(__APPLE__) && defined(__MACH__)
// https://web.archive.org/web/20191012035921/http://nadeausoftware.com/articles/2012/01/c_c_tip_how_use_compiler_predefined_macros_detect_operating_system
#include <sys/param.h>
#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
// geteuid
#include <unistd.h>
#include <sys/types.h>
#endif


bool fs_is_macos(){
#if defined(__APPLE__) && defined(__MACH__)
  return true;
#else
  return false;
#endif
}

bool fs_is_linux() {
#ifdef __linux__
  return true;
#else
  return false;
#endif
}

bool fs_is_unix() {
#ifdef __unix__
  return true;
#else
  return fs_is_macos();
#endif
}

bool fs_is_bsd() {
#ifdef BSD
  return true;
#else
  return false;
#endif
}

bool fs_is_windows() {
#ifdef _WIN32
  return true;
#else
  return false;
#endif
}

bool fs_is_cygwin(){
#ifdef __CYGWIN__
  return true;
#else
  return false;
#endif
}

bool fs_is_mingw(){
#ifdef __MINGW32__
  return true;
#else
  return false;
#endif
}

char fs_pathsep(){
  return fs_is_windows() ? ';' : ':';
}


bool fs_is_admin(){
  // running as admin / root / superuser
#ifdef _WIN32
	HANDLE hToken = NULL;
	TOKEN_ELEVATION elevation;
	DWORD dwSize;

	if(OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)){
	  if(GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwSize)){
      CloseHandle(hToken);
      return elevation.TokenIsElevated;
    }
  }

  if (hToken) CloseHandle(hToken);
  return false;

#else
  return geteuid() == 0;
#endif
}
