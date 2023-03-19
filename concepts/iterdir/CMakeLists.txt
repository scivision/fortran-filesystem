cmake_minimum_required(VERSION 3.20)
project(IterDir LANGUAGES CXX)
enable_testing()

find_package(Python COMPONENTS Interpreter)

add_executable(iterdir iterdir.cpp)
add_test(NAME IterDirCPP COMMAND iterdir "${CMAKE_SOURCE_DIR}")

if(Python_Interpreter_FOUND)
  add_test(NAME IterDirPython
  COMMAND Python::Interpreter ${CMAKE_CURRENT_SOURCE_DIR}/iterdir.py "${CMAKE_SOURCE_DIR}"
  )
endif()

file(GENERATE OUTPUT .gitignore CONTENT "*")
