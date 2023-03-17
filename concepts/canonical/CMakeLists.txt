cmake_minimum_required(VERSION 3.20)

project(ResolvePath LANGUAGES C CXX)
enable_testing()

option(fortran "Enable Fortran" ON)

if(fortran)
  enable_language(Fortran)
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_library(canonical_c canonical.c)
target_include_directories(canonical_c PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(canonical_c PRIVATE $<$<BOOL:${MSVC}>:_CRT_SECURE_NO_WARNINGS>)

add_executable(main_c main.c)
target_link_libraries(main_c PRIVATE canonical_c)

add_test(NAME canonical_link_c
COMMAND ${CMAKE_COMMAND}
  -Dexe:FILEPATH=$<TARGET_FILE:main_c>
  -P ${CMAKE_CURRENT_SOURCE_DIR}/equivalent.cmake
)

string(RANDOM LENGTH 8 ALPHABET "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" Uname)
string(TOLOWER ${Uname} Lname)
string(PREPEND Uname ${CMAKE_CURRENT_LIST_DIR}/cmake_)
string(PREPEND Lname ${CMAKE_CURRENT_LIST_DIR}/cmake_)

add_test(NAME canonical_case_c
COMMAND ${CMAKE_COMMAND}
  -Dexe:FILEPATH=$<TARGET_FILE:main_c>
  -P ${CMAKE_CURRENT_SOURCE_DIR}/case_sense.cmake
)

if(fortran)
  add_executable(exe_dir main.f90)
  target_link_libraries(exe_dir PRIVATE canonical_c)
  add_test(NAME exe_dir COMMAND exe_dir)
endif()

add_executable(canonical_cpp canonical.cpp)

add_test(NAME canonical_link_cpp
COMMAND ${CMAKE_COMMAND}
  -Dexe:FILEPATH=$<TARGET_FILE:canonical_cpp>
  -P ${CMAKE_CURRENT_SOURCE_DIR}/equivalent.cmake
)

add_test(NAME canonical_case_cpp
COMMAND ${CMAKE_COMMAND}
  -Dexe:FILEPATH=$<TARGET_FILE:canonical_cpp>
  -P ${CMAKE_CURRENT_SOURCE_DIR}/case_sense.cmake
)

if(NOT DEFINED case_sensitive)
file(TOUCH ${Lname})
if(EXISTS ${Uname})
  set(case_sensitive CACHE BOOL "case insensitive build filesystem" false)
else()
  set(case_sensitive CACHE BOOL "case sensitive build filesystem" true)
endif()
file(REMOVE ${Uname})
endif()

set_property(TEST canonical_case_c canonical_case_cpp PROPERTY DISABLED ${case_sensitive})

file(GENERATE OUTPUT .gitignore CONTENT "*")
