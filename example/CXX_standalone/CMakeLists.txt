# show that for C++, we only need ffilesystem.h, filesystem.cpp, common.c

cmake_minimum_required(VERSION 3.14)

project(CXX_standalone LANGUAGES C CXX)
# need to enable C language to allow extern "C" functions in C++ code--else linker failure

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 99)

add_library(ffilesystem ../../src/common/filesystem.cpp ../../src/common/common.c)

target_include_directories(ffilesystem PUBLIC ../../include)

add_executable(main ../2/main.cpp)
target_link_libraries(main PRIVATE ffilesystem)

add_test(NAME CXX_standalone COMMAND main)

file(GENERATE OUTPUT .gitignore CONTENT "*")
