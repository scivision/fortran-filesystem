cmake_minimum_required(VERSION 3.20)

string(RANDOM LENGTH 8 ALPHABET "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" Uname)
string(TOLOWER ${Uname} Lname)
string(PREPEND Uname ${CMAKE_CURRENT_LIST_DIR}/cmake_)
string(PREPEND Lname ${CMAKE_CURRENT_LIST_DIR}/cmake_)

message(STATUS "touch file ${Lname}")
file(TOUCH ${Lname})

execute_process(COMMAND ${exe} "${Uname}"
OUTPUT_VARIABLE out OUTPUT_STRIP_TRAILING_WHITESPACE
)

file(REMOVE ${Lname})

cmake_path(COMPARE "${Lname}" EQUAL "${out}" ok)

if(ok)
  message(STATUS "OK: canonical(${Uname}) lexicographically equal ${out}")
else()
  message(FATAL_ERROR "canonical(${Uname}) does not lexicographically equal ${out}")
endif()
