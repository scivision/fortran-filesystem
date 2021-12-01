cmake_minimum_required(VERSION 3.15...3.22)

project(FortranPathlib LANGUAGES Fortran)

include(CTest)

include(cmake/compilers.cmake)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

add_subdirectory(src)
