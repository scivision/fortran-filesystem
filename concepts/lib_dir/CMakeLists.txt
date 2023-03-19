cmake_minimum_required(VERSION 3.20)

# shared: prints full path to shared library file  e.g. /path/to/mylib.[dylib,so,dll]
# static: prints full path to main executable file  e.g.  /path/to/main.exe

project(concept_libdir LANGUAGES C)
enable_testing()

option(fortran "Build Fortran example" ON)

if(fortran)
  enable_language(Fortran)
endif()

include(CheckSymbolExists)
include(GNUInstallDirs)

set(CMAKE_C_STANDARD 99)

# Necessary for shared library with Visual Studio / Windows oneAPI -- creates .lib file to acommpany .dll
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS true)

# Rpath options necessary for shared library install to work correctly in user projects
set(CMAKE_INSTALL_NAME_DIR ${CMAKE_INSTALL_FULL_LIBDIR})
set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_FULL_LIBDIR})
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH true)

file(GENERATE OUTPUT .gitignore CONTENT "*")


if(NOT WIN32)
  set(CMAKE_REQUIRED_LIBRARIES ${CMAKE_DL_LIBS})
  if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CMAKE_REQUIRED_DEFINITIONS -D_GNU_SOURCE)
  endif()
  check_symbol_exists(dladdr "dlfcn.h" HAVE_DLADDR)
  if(NOT HAVE_DLADDR)
    message(FATAL_ERROR "dladdr() not found in dl library ${CMAKE_DL_LIBS}")
  endif()
endif()

add_library(mylib SHARED lib.c)
if(WIN32)
  target_compile_definitions(mylib PRIVATE MY_DLL_NAME="$<TARGET_FILE_NAME:mylib>")
else()
  target_link_libraries(mylib PRIVATE ${CMAKE_DL_LIBS})
endif()

if(fortran)
add_executable(lib_dir main.f90)
target_link_libraries(lib_dir PRIVATE mylib)
add_test(NAME lib_dir COMMAND lib_dir)
endif()
