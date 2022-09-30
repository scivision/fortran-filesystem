cmake_minimum_required(VERSION 3.1...3.25)

project(abi_check LANGUAGES C)

enable_testing()

option(cpp "use C++" true)
option(fortran "use Fortran" true)

if(cpp)
  enable_language(CXX)
endif()
if(fortran)
  enable_language(Fortran)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if((CMAKE_C_COMPILER_ID STREQUAL "GNU" AND CMAKE_C_COMPILER_VERSION VERSION_LESS "9.1.0") OR
   (CMAKE_Fortran_COMPILER_ID STREQUAL "GNU" AND CMAKE_Fortran_COMPILER_VERSION VERSION_LESS "9.1.0") OR
    CMAKE_C_COMPILER_ID STREQUAL "NVHPC")
  set(stdfs stdc++fs)
endif()

# test libraries
add_library(addone OBJECT addone.c)
add_library(addtwo OBJECT "$<IF:$<BOOL:${cpp}>,addtwo.cpp,addtwo.c>")
add_library(addone_f OBJECT "$<IF:$<BOOL:${fortran}>,addone.f90,addone.c>")

# Fortran main using C and C++ libraries
if(fortran)
  add_executable(main_f main.f90 $<TARGET_OBJECTS:addone> $<TARGET_OBJECTS:addtwo>)
  set_property(TARGET main_f PROPERTY LINKER_LANGUAGE Fortran)
  add_test(NAME Fortran_main COMMAND main_f)
endif()

# C++ main using Fortran library
if(cpp)
  add_executable(main_cpp main.cpp $<TARGET_OBJECTS:addone_f>)
  add_test(NAME Cpp_main COMMAND main_cpp)

  add_executable(c_fs main_fs.c lib_fs.cpp $<TARGET_OBJECTS:addone_f>)
  target_link_libraries(c_fs PRIVATE $<$<BOOL:${stdfs}>:${stdfs}>)
  add_test(NAME C_fs COMMAND c_fs)
endif()

# C main using C++ and Fortran library.
# This can be an issue mixing Clang and Gfortran

add_executable(main_c main.c $<TARGET_OBJECTS:addone_f> $<TARGET_OBJECTS:addtwo>)
set_property(TARGET main_c PROPERTY LINKER_LANGUAGE C)
add_test(NAME C_main COMMAND main_c)

# test properties
get_property(test_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY TESTS)
set_property(TEST ${test_names} PROPERTY TIMEOUT 10)
