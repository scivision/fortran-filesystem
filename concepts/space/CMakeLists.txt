cmake_minimum_required(VERSION 3.20)
project(spaced LANGUAGES C CXX)
enable_testing()

set(CMAKE_CXX_STANDARD 20)

add_executable(demo_space demo_space.cpp)
target_compile_options(demo_space PRIVATE $<$<AND:$<CXX_COMPILER_ID:GNU>,$<VERSION_LESS:$<CXX_COMPILER_VERSION>,10.0.0>>:-fconcepts>)
# add_test(NAME demo_space COMMAND demo_space)
# can be locale problems

add_executable(cpp_space_avail space_avail.cpp)
add_test(NAME cpp_space_avail COMMAND cpp_space_avail ${CMAKE_CURRENT_SOURCE_DIR})

file(GENERATE OUTPUT .gitignore CONTENT "*")
