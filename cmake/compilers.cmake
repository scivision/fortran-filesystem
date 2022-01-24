include(CheckIncludeFileCXX)
include(CheckCXXSymbolExists)
include(CheckSourceRuns)

set(libfs)
if(CMAKE_CXX_COMPILER_ID STREQUAL GNU AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.1.0)
  set(libfs stdc++fs)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL Clang AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0.0)
# https://releases.llvm.org/9.0.0/projects/libcxx/docs/UsingLibcxx.html#using-filesystem
  # set(libfs c++fs)  # /usr/bin/ld: cannot find -lc++fs  also happens in Meson
endif()

set(CMAKE_REQUIRED_LIBRARIES ${libfs})

check_cxx_symbol_exists(__cpp_lib_filesystem filesystem HAVE_CXXFS_MACRO)
if(HAVE_CXXFS_MACRO)
  check_include_file_cxx(filesystem HAVE_CXX17_FILESYSTEM)
else()
  check_include_file_cxx(experimental/filesystem HAVE_CXX17_EXPERIMENTAL_FILESYSTEM)
endif()

# --- C++17 filesystem or C lstat() symbolic link information
if(HAVE_CXX17_FILESYSTEM OR HAVE_CXX17_EXPERIMENTAL_FILESYSTEM)
  file(READ ${CMAKE_CURRENT_LIST_DIR}/check_fs_symlink.cpp symlink_src)
  check_source_runs(CXX "${symlink_src}" HAVE_SYMLINK)
endif()

# fixes errors about needing -fPIE
if(CMAKE_SYSTEM_NAME STREQUAL Linux AND CMAKE_CXX_COMPILER_ID STREQUAL Clang)
  set(CMAKE_POSITION_INDEPENDENT_CODE true)
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
