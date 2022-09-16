cmake_minimum_required(VERSION 3.18)
# shared: prints full path to shared library file  e.g. /path/to/mylib.[dylib,so,dll]
# static: prints full path to main executable file  e.g.  /path/to/main.exe

## MSYS2 has libdl: mingw-w64-x86_64-dlfcn

project(concept_libdir LANGUAGES C Fortran)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

# --- auto-ignore build directory
if(NOT EXISTS ${PROJECT_BINARY_DIR}/.gitignore)
  file(WRITE ${PROJECT_BINARY_DIR}/.gitignore "*")
endif()

find_path(DL_INCDIR NAMES dlfcn.h REQUIRED)

find_library(DL_LIBRARY
NAMES dl
HINTS ${CMAKE_DL_LIBS}
REQUIRED
)

add_library(mylib lib.c)
target_include_directories(mylib PRIVATE ${DL_INCDIR})
target_link_libraries(mylib PRIVATE ${DL_LIBRARY})

add_executable(lib_dir main.f90)
target_include_directories(lib_dir PRIVATE ${DL_INCDIR})
target_link_libraries(lib_dir PRIVATE mylib)
