cmake_minimum_required(VERSION 3.11)

project(concept_exedir LANGUAGES C Fortran)

file(GENERATE OUTPUT .gitignore CONTENT "*")

add_executable(exec exec.c)

if(WIN32)
  add_executable(CreateProcess CreateProcess.c)
endif()
