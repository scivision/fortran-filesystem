cmake_minimum_required(VERSION 3.11)

project(concepts LANGUAGES C Fortran)

# --- auto-ignore build directory
if(NOT EXISTS ${PROJECT_BINARY_DIR}/.gitignore)
  file(WRITE ${PROJECT_BINARY_DIR}/.gitignore "*")
endif()

add_executable(exe_dir realpath.c exe_dir.f90)
