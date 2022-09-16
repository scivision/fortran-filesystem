cmake_minimum_required(VERSION 3.15...3.25)
# 3.15+: TARGET_FILE_BASE_NAME

# shared: prints full path to shared library file  e.g. /path/to/mylib.[dylib,so,dll]
# static: prints full path to main executable file  e.g.  /path/to/main.exe

## MSYS2 has libdl: mingw-w64-x86_64-dlfcn

project(concept_libdir LANGUAGES C Fortran)

include(CheckFunctionExists)
include(GNUInstallDirs)

# Necessary for shared library with Visual Studio / Windows oneAPI -- creates .lib file to acommpany .dll
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS true)

# Rpath options necessary for shared library install to work correctly in user projects
set(CMAKE_INSTALL_NAME_DIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH true)

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

set(CMAKE_REQUIRED_INCLUDES ${DL_INCDIR})
set(CMAKE_REQUIRED_LIBRARIES ${DL_LIBRARY})
set(CMAKE_REQUIRED_DEFINITIONS -D_GNU_SOURCE)
check_function_exists(dladdr HAVE_DLADDR)

if(NOT HAVE_DLADDR)
  message(FATAL_ERROR "dladdr() not found in dl library ${DL_LIBRARY}")
endif()

endif()

add_library(mylib SHARED lib.c)
target_compile_features(mylib PRIVATE c_std_99)
if(MSVC)
  target_compile_definitions(mylib PRIVATE MY_DLL_NAME="$<TARGET_FILE_BASE_NAME:mylib>.dll")
else()
  target_include_directories(mylib PRIVATE ${DL_INCDIR})
  target_link_libraries(mylib PRIVATE ${DL_LIBRARY})
  target_compile_definitions(mylib PRIVATE _GNU_SOURCE)
endif()

add_executable(lib_dir main.f90)
target_link_libraries(lib_dir PRIVATE mylib)
