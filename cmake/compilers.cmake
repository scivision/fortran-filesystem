include(CheckIncludeFile)
include(CheckSymbolExists)
include(CheckCXXSymbolExists)
include(CheckSourceCompiles)
include(CheckSourceRuns)

if(CPP_FS)
  check_cxx_symbol_exists(__cpp_lib_filesystem filesystem HAVE_CXXFS_MACRO)
  if(HAVE_CXXFS_MACRO)
    check_source_runs(CXX
    [=[
    #include <filesystem>
    int main(){ bool e = std::filesystem::exists("."); return 0; }
    ]=]
    HAVE_CXX17_FILESYSTEM
    )
  endif()
endif()
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
if(HAVE_CXX17_FILESYSTEM)
  file(READ ${CMAKE_CURRENT_LIST_DIR}/check_fs_symlink.cpp symlink_src)
  check_source_runs(CXX "${symlink_src}" HAVE_SYMLINK)
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
