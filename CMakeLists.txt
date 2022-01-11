cmake_minimum_required(VERSION 3.19...3.22)

project(FortranPathlib
LANGUAGES C Fortran
VERSION 2.4.0
DESCRIPTION "Platform independent, object-oriented Fortran filesystem path manipulation library"
HOMEPAGE_URL "https://github.com/scivision/fortran-pathlib"
)

include(CTest)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

include(cmake/compilers.cmake)
include(cmake/options.cmake)

add_library(pathlib)
set_target_properties(pathlib PROPERTIES
Fortran_MODULE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include
)
target_include_directories(pathlib PUBLIC
$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
$<INSTALL_INTERFACE:include>
)
add_subdirectory(src)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/pathlib.mod
TYPE INCLUDE
)

install(TARGETS pathlib EXPORT ${PROJECT_NAME}-targets)

# --- CLI

if(BUILD_UTILS)
  add_executable(pathlib_cli src/cli.f90)
  target_link_libraries(pathlib_cli PRIVATE pathlib)

  install(TARGETS pathlib_cli EXPORT ${PROJECT_NAME}-targets)
endif()

# --- Tests
if(BUILD_TESTING)
  add_subdirectory(src/tests)
endif()

include(cmake/install.cmake)
