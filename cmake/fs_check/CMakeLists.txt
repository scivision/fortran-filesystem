cmake_minimum_required(VERSION 3.1...3.25)

project(fs_check LANGUAGES C CXX)

if(fortran)
  enable_language(Fortran)
endif()

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(c_fs main_fs.c lib_fs.cpp
$<$<BOOL:${fortran}>:lib.f90>
)
target_link_libraries(c_fs PRIVATE ${GNU_stdfs})
add_test(NAME C_fs COMMAND c_fs)
