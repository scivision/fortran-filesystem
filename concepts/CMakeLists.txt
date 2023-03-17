cmake_minimum_required(VERSION 3.20)
project(FilesystemConcepts LANGUAGES C CXX)
enable_testing()

set(CMAKE_CXX_STANDARD 20)

foreach(d IN ITEMS canonical exe_dir file_types iterdir lib_dir space syscall)
  add_subdirectory(${d})
endforeacH()

file(GENERATE OUTPUT .gitignore CONTENT "*")
