cmake_minimum_required(VERSION 3.11...3.25)
# shared: prints full path to shared library file  e.g. /path/to/mylib.[dylib,so,dll]
# static: prints full path to main executable file  e.g.  /path/to/main.exe

## MSYS2 has libdl: mingw-w64-x86_64-dlfcn

project(concept_libdir LANGUAGES C Fortran)

# Necessary for shared library with Visual Studio / Windows oneAPI -- creates .lib file to acommpany .dll
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS true)

# --- auto-ignore build directory
if(NOT EXISTS ${PROJECT_BINARY_DIR}/.gitignore)
  file(WRITE ${PROJECT_BINARY_DIR}/.gitignore "*")
endif()

if(NOT MSVC)
find_path(DL_INCDIR NAMES dlfcn.h)

find_library(DL_LIBRARY
NAMES dl
HINTS ${CMAKE_DL_LIBS}
)
endif()

add_library(mylib SHARED lib.c)
target_include_directories(mylib PRIVATE $<$<NOT:$<BOOL:${MSVC}>>:${DL_INCDIR}>)
target_link_libraries(mylib PRIVATE $<$<NOT:$<BOOL:${MSVC}>>:${DL_LIBRARY}>)
target_compile_features(mylib PRIVATE c_std_99)

add_executable(lib_dir main.f90)
target_link_libraries(lib_dir PRIVATE mylib)
