cmake_minimum_required(VERSION 3.11)

project(concept_exedir LANGUAGES C Fortran)

# --- auto-ignore build directory
if(NOT EXISTS ${PROJECT_BINARY_DIR}/.gitignore)
  file(WRITE ${PROJECT_BINARY_DIR}/.gitignore "*")
endif()

add_executable(exe_dir realpath.c main.f90)
