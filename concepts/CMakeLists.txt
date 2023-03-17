cmake_minimum_required(VERSION 3.20)
project(FilesystemConcepts LANGUAGES C CXX)
enable_testing()

option(fortran "use Fortran" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

foreach(d IN ITEMS canonical file_types iterdir lib_dir space syscall)
  add_subdirectory(${d})
endforeacH()

file(GENERATE OUTPUT .gitignore CONTENT "*")
