cmake_minimum_required(VERSION 3.19...3.24)
# 3.19 for CONFIG:list genex

project(ffilesystem
LANGUAGES C CXX Fortran
VERSION 3.6.0
DESCRIPTION "Platform independent, object-oriented Fortran filesystem path manipulation library"
HOMEPAGE_URL "https://github.com/scivision/fortran-filesystem"
)

include(CTest)

include(cmake/options.cmake)
include(cmake/compilers.cmake)

if(fallback)
  add_subdirectory(src/fallback)
else()
  add_subdirectory(src)
  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/filesystem.h TYPE INCLUDE)
endif()

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/filesystem.mod TYPE INCLUDE)

install(TARGETS filesystem EXPORT ${PROJECT_NAME}-targets)

# --- CLI

if(BUILD_UTILS)
  add_executable(filesystem_cli app/main.f90)
  target_link_libraries(filesystem_cli PRIVATE filesystem)
  set_property(TARGET filesystem_cli PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

  install(TARGETS filesystem_cli EXPORT ${PROJECT_NAME}-targets)
endif()

# --- Tests
if(BUILD_TESTING)
  add_subdirectory(test)
endif()

include(cmake/install.cmake)
