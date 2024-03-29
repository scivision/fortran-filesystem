#include <stdbool.h>
#include <string.h>
#include <stdio.h>

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

	if(OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken) &&
     GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwSize)){
    CloseHandle(hToken);
    return elevation.TokenIsElevated;
  }

  if (hToken) CloseHandle(hToken);
  return false;

#else
  return geteuid() == 0;
#endif
}


size_t fs_compiler(char* name, size_t buffer_size)
{
int L=0;

#if defined(__INTEL_LLVM_COMPILER)
  L = snprintf(name, buffer_size, "Intel LLVM %d %s", __INTEL_LLVM_COMPILER,  __VERSION__);
#elif defined(__NVCOMPILER_LLVM__)
  L = snprintf(name, buffer_size, "NVIDIA nvc %d.%d.%d", __NVCOMPILER_MAJOR__, __NVCOMPILER_MINOR__, __NVCOMPILER_PATCHLEVEL__);
#elif defined(__clang__)
  #ifdef __VERSION__
    L = snprintf(name, buffer_size, "Clang %s", __VERSION__);
  #else
    L = snprintf(name, buffer_size, "Clang %d.%d.%d", __clang_major__, __clang_minor__, __clang_patchlevel__);
  #endif
#elif defined(__GNUC__)
  L = snprintf(name, buffer_size, "GNU GCC %d.%d.%d", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#elif defined(_MSC_VER)
  L = snprintf(name, buffer_size, "MSVC %d", _MSC_FULL_VER);
#else
  name[0] = '\0';
#endif

if (L < 0){ // cppcheck-suppress knownConditionTrueFalse
  fprintf(stderr, "ERROR:ffilesystem:fs_compiler: snprintf failed\n");
  L = 0;
}
if((size_t)L >= buffer_size){  // cppcheck-suppress unsignedLessThanZero
  L = buffer_size-1;
  name[L] = '\0';
}

  return L;
}
