cmake_minimum_required(VERSION 3.19...3.22)
# 3.19 for CONFIG:list genex

project(FortranFilesystem
LANGUAGES C CXX Fortran
VERSION 3.1.1
DESCRIPTION "Platform independent, object-oriented Fortran filesystem path manipulation library"
HOMEPAGE_URL "https://github.com/scivision/fortran-filesystem"
)

include(CTest)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)


set(CMAKE_CXX_STANDARD 17)

include(cmake/options.cmake)
include(cmake/compilers.cmake)

# --- filesystem binary
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

add_library(filesystem
src/filesystem.f90 src/iter.f90 src/io.f90 src/find.f90
${CMAKE_CURRENT_BINARY_DIR}/platform.f90
)
set_target_properties(filesystem PROPERTIES
Fortran_MODULE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include
)
target_include_directories(filesystem PUBLIC
$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
$<INSTALL_INTERFACE:include>
)

# --- C++17 filesystem OR lstat() symbolic link information

if(HAVE_CXX17_FILESYSTEM OR HAVE_CXX17_EXPERIMENTAL_FILESYSTEM)
  add_library(filesystem_cpp OBJECT src/fs.cpp)
  target_compile_features(filesystem_cpp PRIVATE cxx_std_17)
  target_compile_definitions(filesystem_cpp PRIVATE $<$<BOOL:${MSVC}>:_CRT_SECURE_NO_WARNINGS>)

  target_sources(filesystem PRIVATE src/fs_cpp.f90 $<TARGET_OBJECTS:filesystem_cpp>)
  target_link_libraries(filesystem PUBLIC ${libfs})
else()
  message(FATAL_ERROR "C++17 filesystem support is required for Fortran filesystem")
endif()


install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/filesystem.mod
TYPE INCLUDE
)

install(TARGETS filesystem EXPORT ${PROJECT_NAME}-targets)

# --- CLI

if(BUILD_UTILS)
  add_executable(filesystem_cli src/cli.f90)
  target_link_libraries(filesystem_cli PRIVATE filesystem)

  install(TARGETS filesystem_cli EXPORT ${PROJECT_NAME}-targets)
endif()

# --- Tests
if(BUILD_TESTING)
  add_subdirectory(src/tests)
endif()

include(cmake/install.cmake)
