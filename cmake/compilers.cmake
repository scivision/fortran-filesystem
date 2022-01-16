include(CheckIncludeFile)
include(CheckSymbolExists)
include(CheckCXXSymbolExists)
include(CMakeDependentOption)
include(CheckSourceCompiles)
include(CheckSourceRuns)

check_cxx_symbol_exists(__cpp_lib_filesystem filesystem HAVE_CXX17_FILESYSTEM)

cmake_dependent_option(CPP_FS "use C++17 filesystem" true ${HAVE_CXX17_FILESYSTEM} false)

# --- utime() update file time

check_include_file(utime.h HAVE_UTIME_H)
if(HAVE_UTIME_H)
  check_symbol_exists(utime utime.h HAVE_UTIME)
else()
  check_include_file(sys/utime.h HAVE_SYS_UTIME_H)
  if(HAVE_SYS_UTIME_H)
    if(WIN32)
      check_symbol_exists(_utime sys/utime.h HAVE_WIN32_UTIME)
    else()
      check_symbol_exists(utime sys/utime.h HAVE_UTIME)
    endif()
  endif()
endif()

# --- C++17 filesystem or C lstat() symbolic link information

if(CPP_FS)
  file(READ ${CMAKE_CURRENT_LIST_DIR}/check_fs_symlink.cpp symlink_src)
  check_source_runs(CXX "${symlink_src}" HAVE_SYMLINK)
else()
  check_include_file(sys/stat.h HAVE_SYS_STAT_H)
  if(HAVE_SYS_STAT_H)
    check_symbol_exists(lstat sys/stat.h HAVE_LSTAT)
  endif()
  if(HAVE_LSTAT)
    file(READ ${CMAKE_CURRENT_LIST_DIR}/check_lstat.f90 lstat_src)
    check_source_compiles(Fortran ${lstat_src} HAVE_SYMLINK)
  endif()
endif()

# --- flags

if(CMAKE_Fortran_COMPILER_ID MATCHES "^Intel")

add_compile_options(
$<IF:$<BOOL:${WIN32}>,/QxHost,-xHost>
"$<$<COMPILE_LANGUAGE:Fortran>:-warn;-heap-arrays>"
"$<$<AND:$<COMPILE_LANGUAGE:Fortran>,$<CONFIG:Debug,RelWithDebInfo>>:-traceback;-check;-debug>"
)

elseif(CMAKE_Fortran_COMPILER_ID STREQUAL "GNU")

add_compile_options(
-mtune=native -Wall
$<$<CONFIG:Debug,RelWithDebInfo>:-Wextra>
"$<$<COMPILE_LANGUAGE:Fortran>:-Wno-intrinsic-shadow;-fimplicit-none>"
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
