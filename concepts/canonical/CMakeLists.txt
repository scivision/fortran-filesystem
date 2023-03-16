cmake_minimum_required(VERSION 3.12)

project(ResolvePath LANGUAGES C CXX)


set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(canonical_c canonical.c)

add_executable(canonical_cpp canonical.cpp)
