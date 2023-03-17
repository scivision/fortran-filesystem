cmake_minimum_required(VERSION 3.20)

project(ResolvePath LANGUAGES C CXX)
enable_testing()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(canonical_c canonical.c)
target_compile_definitions(canonical_c PRIVATE $<$<BOOL:${MSVC}>:_CRT_SECURE_NO_WARNINGS>)
add_test(NAME canonical_c
COMMAND ${CMAKE_COMMAND}
  -Dexe:FILEPATH=$<TARGET_FILE:canonical_c>
  -P ${CMAKE_CURRENT_SOURCE_DIR}/equivalent.cmake
)

add_executable(canonical_cpp canonical.cpp)
add_test(NAME canonical_cpp
COMMAND ${CMAKE_COMMAND}
  -Dexe:FILEPATH=$<TARGET_FILE:canonical_cpp>
  -P ${CMAKE_CURRENT_SOURCE_DIR}/equivalent.cmake
)

file(GENERATE OUTPUT .gitignore CONTENT "*")
