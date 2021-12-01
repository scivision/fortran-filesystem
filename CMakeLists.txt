cmake_minimum_required(VERSION 3.15...3.22)

project(FortranPathlib LANGUAGES Fortran)

include(CTest)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

include(cmake/compilers.cmake)
include(cmake/options.cmake)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_Fortran_MODULE_DIRECTORY ${PROJECT_BINARY_DIR}/include)

if(advanced)
  include(CheckLanguage)
  check_language(CXX)
  if(CMAKE_CXX_COMPILER)
    enable_language(CXX)
  else()
    message(STATUS "No C++ support")
    set(advanced false)
  endif()
endif()

add_subdirectory(src)

install(TARGETS pathlib EXPORT ${PROJECT_NAME}-targets)
install(FILES ${PROJECT_BINARY_DIR}/include/pathlib.mod TYPE INCLUDE)
