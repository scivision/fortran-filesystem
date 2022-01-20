cmake_minimum_required(VERSION 3.19...3.22)

project(FortranPathlib
LANGUAGES C CXX Fortran
VERSION 3.0.0
DESCRIPTION "Platform independent, object-oriented Fortran filesystem path manipulation library"
HOMEPAGE_URL "https://github.com/scivision/fortran-pathlib"
)

include(CTest)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

set(CMAKE_CXX_STANDARD 17)

include(cmake/options.cmake)
include(cmake/compilers.cmake)

# --- pathlib binary
if(HAVE_SYMLINK)
  set(has_symlink .true.)
else()
  set(has_symlink .false.)
endif()
configure_file(src/symlink.in.f90 symlink.f90 @ONLY)

add_library(pathlib
src/pathlib.f90 src/iter.f90 src/io.f90 src/find.f90
${CMAKE_CURRENT_BINARY_DIR}/symlink.f90
)
set_target_properties(pathlib PROPERTIES
Fortran_MODULE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include
)
target_include_directories(pathlib PUBLIC
$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
$<INSTALL_INTERFACE:include>
)




# --- C++17 filesystem OR lstat() symbolic link information

if(HAVE_CXX17_FILESYSTEM)
  target_sources(pathlib PRIVATE src/fs_cpp.f90 src/fs.cpp)
  target_compile_definitions(pathlib PRIVATE $<$<BOOL:${MSVC}>:_CRT_SECURE_NO_WARNINGS>)
  target_link_libraries(pathlib PUBLIC ${libfs})
else()
  add_subdirectory(fallback)
endif()


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
