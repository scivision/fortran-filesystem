cmake_minimum_required(VERSION 3.5)

project(fs_check LANGUAGES C CXX)

if(fortran)
  enable_language(Fortran)
endif()

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(fs_cpp OBJECT lib_fs.cpp)
target_link_libraries(fs_cpp PRIVATE ${GNU_stdfs})

add_executable(c_fs main_fs.c $<TARGET_OBJECTS:fs_cpp>)
add_test(NAME C_fs COMMAND c_fs)

if(fortran)
  add_executable(fortran_fs main.f90 $<TARGET_OBJECTS:fs_cpp>)
  add_test(NAME Fortran_fs COMMAND fortran_fs)
endif()
