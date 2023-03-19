cmake_minimum_required(VERSION 3.20)

project(ResolvePath LANGUAGES C CXX)
enable_testing()

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "In-source builds are not allowed.")
endif()

include(CheckCXXSymbolExists)

option(fortran "Enable Fortran" ON)

if(fortran)
  enable_language(Fortran)
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

check_cxx_symbol_exists(__cpp_lib_starts_ends_with "string" HAVE_STRING_START_WITH)

# --- C

add_library(canonical_c canonical.c
$<$<BOOL:${WIN32}>:windows_read_symlink.c>
)
target_include_directories(canonical_c PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(canonical_c PRIVATE $<$<BOOL:${MSVC}>:_CRT_SECURE_NO_WARNINGS>)

add_executable(main_c main.c)
target_link_libraries(main_c PRIVATE canonical_c)

add_test(NAME canonical_link_c
COMMAND ${CMAKE_COMMAND}
  -Dexe:FILEPATH=$<TARGET_FILE:main_c>
  -P ${CMAKE_CURRENT_SOURCE_DIR}/equivalent.cmake
)

add_test(NAME canonical_case_c
COMMAND ${CMAKE_COMMAND}
  -Dexe:FILEPATH=$<TARGET_FILE:main_c>
  -P ${CMAKE_CURRENT_SOURCE_DIR}/case_sense.cmake
)

# --- fortran

if(fortran)
  add_executable(exe_dir main.f90)
  target_link_libraries(exe_dir PRIVATE canonical_c)
  add_test(NAME exe_dir COMMAND exe_dir)
endif()

# --- C++

add_executable(canonical_cpp canonical.cpp
$<$<BOOL:${MINGW}>:windows_read_symlink.cpp>
)
target_include_directories(canonical_cpp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

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

# --- case test file

string(RANDOM LENGTH 8 ALPHABET "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" Uname)
string(TOLOWER ${Uname} Lname)
string(PREPEND Uname ${CMAKE_CURRENT_LIST_DIR}/cmake_)
string(PREPEND Lname ${CMAKE_CURRENT_LIST_DIR}/cmake_)

if(NOT DEFINED case_sensitive)
file(TOUCH ${Lname})
if(EXISTS ${Uname})
  set(case_sensitive false CACHE BOOL "case insensitive build filesystem")
else()
  set(case_sensitive true CACHE BOOL "case sensitive build filesystem")
endif()
file(REMOVE ${Uname})
endif()

set_property(TEST canonical_case_c canonical_case_cpp PROPERTY DISABLED ${case_sensitive})

file(GENERATE OUTPUT .gitignore CONTENT "*")
