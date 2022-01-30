include(CheckIncludeFileCXX)
include(CheckCXXSymbolExists)
include(CheckSourceRuns)
include(CheckSourceCompiles)

# check if Fortran compiler new enough
check_source_compiles(Fortran "
module a
implicit none (type, external)

interface
module subroutine d()
end subroutine d
end interface
end module

submodule (a) b
contains
module procedure d
end procedure d
end submodule

program c
use a, only : d

character :: e
error stop e
end program
"
HAS_Fortran_2018
)
if(NOT HAS_Fortran_2018)
  message(FATAL_ERROR "Fortran compiler ${CMAKE_Fortran_COMPILER_ID} ${CMAKE_Fortran_COMPILER_VERSION} does not support Fortran 2018 syntax")
endif()

# setup / check C++ filesystem

set(libfs)
if(CMAKE_CXX_COMPILER_ID STREQUAL GNU AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.1.0)
  set(libfs stdc++fs)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL Clang AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0.0)
# https://releases.llvm.org/9.0.0/projects/libcxx/docs/UsingLibcxx.html#using-filesystem
  # set(libfs c++fs)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "^Intel" AND CMAKE_SYSTEM_NAME STREQUAL Linux)
  # NOTE: Intel compiler must use GCC >= 9 else you get linker errors, even with -lstdc++fs
  # e.g. on CentOS / RHEL use gcc-toolset-9 or similar
  set(libfs stdc++)
endif()

set(CMAKE_REQUIRED_LIBRARIES ${libfs})

if(MSVC)
  set(CMAKE_REQUIRED_FLAGS /std:c++17)
else()
  set(CMAKE_REQUIRED_FLAGS -std=c++17)
endif()

check_cxx_symbol_exists(__cpp_lib_filesystem filesystem HAVE_CXX17_FILESYSTEM)
if(NOT HAVE_CXX17_FILESYSTEM)
  check_include_file_cxx(experimental/filesystem HAVE_CXX17_EXPERIMENTAL_FILESYSTEM)
endif()

# --- C++17 filesystem or C lstat() symbolic link information
if(HAVE_CXX17_FILESYSTEM OR HAVE_CXX17_EXPERIMENTAL_FILESYSTEM)
  file(READ ${CMAKE_CURRENT_LIST_DIR}/check_fs_symlink.cpp symlink_src)
  check_source_runs(CXX "${symlink_src}" HAVE_SYMLINK)
endif()

# fixes errors about needing -fPIE
if(CMAKE_SYSTEM_NAME STREQUAL Linux)
  if(CMAKE_CXX_COMPILER_ID MATCHES "(Clang|Intel)")
    set(CMAKE_POSITION_INDEPENDENT_CODE true)
  elseif(BUILD_SHARED_LIBS AND CMAKE_CXX_COMPILER_ID STREQUAL GNU)
    set(CMAKE_POSITION_INDEPENDENT_CODE true)
  endif()
endif()

# --- flags

if(CMAKE_Fortran_COMPILER_ID MATCHES "^Intel")

add_compile_options(
$<IF:$<BOOL:${WIN32}>,/QxHost,-xHost>
"$<$<COMPILE_LANGUAGE:Fortran>:-warn;-heap-arrays>"
"$<$<COMPILE_LANGUAGE:C,CXX>:-Wall>"
"$<$<AND:$<COMPILE_LANGUAGE:Fortran>,$<CONFIG:Debug,RelWithDebInfo>>:-traceback;-check;-debug>"
)

elseif(CMAKE_Fortran_COMPILER_ID STREQUAL "GNU")

add_compile_options(
-mtune=native -Wall
$<$<CONFIG:Debug,RelWithDebInfo>:-Wextra>
"$<$<COMPILE_LANGUAGE:Fortran>:-fimplicit-none>"
"$<$<AND:$<COMPILE_LANGUAGE:Fortran>,$<CONFIG:Debug,RelWithDebInfo>>:-fcheck=all;-Werror=array-bounds>"
"$<$<AND:$<COMPILE_LANGUAGE:Fortran>,$<CONFIG:Release>>:-fno-backtrace>"
)

add_compile_options($<$<COMPILE_LANGUAGE:Fortran>:-Wno-maybe-uninitialized>)
# spurious warning on character(:), allocatable :: C

add_compile_options($<$<COMPILE_LANGUAGE:Fortran>:-Wno-uninitialized>)
# spurious warning on character(:), allocatable :: C(:)

endif()

# --- code coverage
if(ENABLE_COVERAGE)
include(CodeCoverage)
append_coverage_compiler_flags()
set(COVERAGE_EXCLUDES ${PROJECT_SOURCE_DIR}/src/tests)
endif()
