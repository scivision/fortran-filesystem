cmake_minimum_required(VERSION 3.12)

project(spaced LANGUAGES C CXX)
enable_testing()

file(GENERATE OUTPUT .gitignore CONTENT "*")

set(CMAKE_CXX_STANDARD 20)

add_executable(demo_space demo_space.cpp)
add_test(NAME demo_space COMMAND demo_space)

add_executable(cpp_space_avail space_avail.cpp)
add_test(NAME cpp_space_avail COMMAND cpp_space_avail ${CMAKE_CURRENT_SOURCE_DIR})
