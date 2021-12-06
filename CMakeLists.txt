cmake_minimum_required(VERSION 3.19...3.22)

project(FortranPathlib
LANGUAGES C Fortran
VERSION 2.3.1
)

include(CTest)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

include(cmake/compilers.cmake)
include(cmake/options.cmake)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_Fortran_MODULE_DIRECTORY ${PROJECT_BINARY_DIR}/include)

add_subdirectory(src)

install(TARGETS pathlib EXPORT ${PROJECT_NAME}-targets)
install(FILES ${PROJECT_BINARY_DIR}/include/pathlib.mod TYPE INCLUDE)
