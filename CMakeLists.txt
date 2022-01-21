cmake_minimum_required(VERSION 3.19...3.22)

project(FortranPathlib
LANGUAGES C CXX Fortran
VERSION 3.1.0
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
if(HAVE_CXX17_FILESYSTEM)
  set(cpp_full_filesystem .true.)
else()
  set(cpp_full_filesystem .false.)
endif()
configure_file(src/platform.in.f90 platform.f90 @ONLY)

add_library(pathlib
src/pathlib.f90 src/iter.f90 src/io.f90 src/find.f90
${CMAKE_CURRENT_BINARY_DIR}/platform.f90
)
set_target_properties(pathlib PROPERTIES
Fortran_MODULE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include
)
target_include_directories(pathlib PUBLIC
$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
$<INSTALL_INTERFACE:include>
)

# fixes errors about needing -fPIE on certain systems:
if(CMAKE_SYSTEM_NAME STREQUAL Linux AND CMAKE_CXX_COMPILER_ID STREQUAL Clang)
  set_target_properties(pathlib PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
endif()

# --- C++17 filesystem OR lstat() symbolic link information

if(HAVE_CXX17_FILESYSTEM OR HAVE_CXX17_EXPERIMENTAL_FILESYSTEM)
  add_library(pathlib_cpp OBJECT src/fs.cpp)
  target_compile_features(pathlib_cpp PRIVATE cxx_std_17)
  target_compile_definitions(pathlib_cpp PRIVATE $<$<BOOL:${MSVC}>:_CRT_SECURE_NO_WARNINGS>)

  target_sources(pathlib PRIVATE src/fs_cpp.f90 $<TARGET_OBJECTS:pathlib_cpp>)
  target_link_libraries(pathlib PUBLIC ${libfs})
elseif(fallback)
  add_subdirectory(fallback)
else()
  message(FATAL_ERROR "C++17 filesystem support is required or enable deprecated fallback with
    cmake -Dfallback=yes"
  )
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
