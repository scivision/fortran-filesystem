cmake_minimum_required(VERSION 3.14)

project(fs_check LANGUAGES C CXX)

option(ffilesystem_fortran "Build Fortran test")

if(ffilesystem_fortran)
  enable_language(Fortran)
endif()

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(GNU_stdfs)
  message(STATUS "fs_check: applying flags ${GNU_stdfs}")
endif()

# fixes errors about needing -fPIE
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  include(CheckPIESupported)
  check_pie_supported()
  set(CMAKE_POSITION_INDEPENDENT_CODE true)
endif()


add_library(fs_cpp OBJECT lib_fs.cpp)
target_link_libraries(fs_cpp PUBLIC ${GNU_stdfs})

add_executable(c_fs main_fs.c)
target_link_libraries(c_fs PRIVATE fs_cpp)
add_test(NAME C_fs COMMAND c_fs)

if(ffilesystem_fortran)
  add_executable(fortran_fs main.f90)
  target_link_libraries(fortran_fs PRIVATE fs_cpp)
  set_property(TARGET fortran_fs PROPERTY LINKER_LANGUAGE Fortran)
  add_test(NAME Fortran_fs COMMAND fortran_fs)
endif()
