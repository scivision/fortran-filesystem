include(CheckSymbolExists)
include(CheckCXXSymbolExists)
include(CheckFortranSourceCompiles)
include(CheckCXXSourceCompiles)
include(CheckCXXSourceRuns)

# --- abi check

# check C and Fortran compiler ABI compatibility

if(NOT abi_ok)
  message(CHECK_START "checking that C, C++, and Fortran compilers can link")
  try_compile(abi_ok
  ${CMAKE_CURRENT_BINARY_DIR}/abi_check ${CMAKE_CURRENT_LIST_DIR}/abi_check
  abi_check
  OUTPUT_VARIABLE abi_log
  )
  if(abi_ok)
    message(CHECK_PASS "OK")
  else()
    message(FATAL_ERROR "ABI-incompatible compilers:
    C compiler ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}
    C++ compiler ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}
    Fortran compiler ${CMAKE_Fortran_COMPILER_ID} ${CMAKE_Fortran_COMPILER_VERSION}
    ${abi_log}
    "
    )
  endif()
endif()

# check if Fortran compiler new enough
check_fortran_source_compiles("
module a
implicit none

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
SRC_EXT f90
)
if(NOT HAS_Fortran_2018)
  message(FATAL_ERROR "Fortran compiler ${CMAKE_Fortran_COMPILER_ID} ${CMAKE_Fortran_COMPILER_VERSION} does not support Fortran 2018 syntax")
endif()

# setup / check C++ filesystem

unset(CMAKE_REQUIRED_LIBRARIES)

if(fallback)

  unset(HAVE_CXX_FILESYSTEM CACHE)

else()

  if((CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.1.0) OR
      CMAKE_CXX_COMPILER_ID STREQUAL "NVHPC")
    set(CMAKE_REQUIRED_LIBRARIES stdc++fs)
  endif()

  if(MSVC)
    set(CMAKE_REQUIRED_FLAGS /std:c++17)
  else()
    set(CMAKE_REQUIRED_FLAGS -std=c++17)
  endif()

  check_cxx_symbol_exists(__has_include "" HAVE_INCLUDE_MACRO)
  if(NOT HAVE_INCLUDE_MACRO)
    message(FATAL_ERROR "C++ compiler not C++17 capable ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
  endif()

  check_cxx_symbol_exists(__cpp_lib_filesystem filesystem HAVE_CXX_FILESYSTEM)

endif()

if(HAVE_CXX_FILESYSTEM)
  message(STATUS "C++ filesystem support enabled.")
else()
  message(STATUS "C++ filesystem support is not available with ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}"
  )
  set(fallback true)
endif()


if(NOT fallback)
  # some compilers e.g. Cray claim to have filesystem, but their libstdc++ doesn't have it.
  check_cxx_source_compiles([=[
  #if __has_include(<filesystem>)
  #include <filesystem>
  namespace fs = std::filesystem;
  #else
  #error "No C++ filesystem support"
  #endif

  int main () {
  fs::path tgt(".");
  auto h = tgt.has_filename();
  return EXIT_SUCCESS;
  }
  ]=]
  HAVE_FILESYSTEM_STDLIB
  )

  if(NOT HAVE_FILESYSTEM_STDLIB)
    message(STATUS "C++ compiler has filesystem, but filesystem is missing from libstdc++ ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}
    Using fallback with limited functionality.")
    set(fallback true)
  endif()

endif()


if(fallback AND NOT fallback_auto)
  message(FATAL_ERROR "filesystem C++ fallback was requested, but not auto-enabled")
endif()


# fixes errors about needing -fPIE
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  if(CMAKE_CXX_COMPILER_ID MATCHES "(Clang|Intel)")
    set(CMAKE_POSITION_INDEPENDENT_CODE true)
  elseif(BUILD_SHARED_LIBS AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_POSITION_INDEPENDENT_CODE true)
  endif()
endif()

# --- flags

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_compile_options($<$<COMPILE_LANGUAGE:C>:-Werror=implicit-function-declaration>)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "(Clang|Intel)")
  add_compile_options(
  "$<$<COMPILE_LANGUAGE:C,CXX>:-Wall;-Wextra>"
  "$<$<COMPILE_LANGUAGE:C>:-Werror=implicit-function-declaration>"
  )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  add_compile_options("$<$<COMPILE_LANGUAGE:C,CXX>:/W3>")
endif()

if(CMAKE_Fortran_COMPILER_ID MATCHES "^Intel")

add_compile_options(
"$<$<COMPILE_LANGUAGE:Fortran>:-warn>"
"$<$<AND:$<COMPILE_LANGUAGE:Fortran>,$<CONFIG:Debug>>:-traceback;-check;-debug>"
)

# -heap-arrays

elseif(CMAKE_Fortran_COMPILER_ID STREQUAL "GNU")

add_compile_options(
-Wall
$<$<CONFIG:Debug>:-Wextra>
"$<$<COMPILE_LANGUAGE:Fortran>:-fimplicit-none>"
"$<$<AND:$<COMPILE_LANGUAGE:Fortran>,$<CONFIG:Debug>>:-fcheck=all;-Werror=array-bounds>"
"$<$<AND:$<COMPILE_LANGUAGE:Fortran>,$<CONFIG:Release>>:-fno-backtrace>"
)

add_compile_options($<$<COMPILE_LANGUAGE:Fortran>:-Wno-maybe-uninitialized>)
# spurious warning on character(:), allocatable :: C

add_compile_options($<$<COMPILE_LANGUAGE:Fortran>:-Wno-uninitialized>)
# spurious warning on character(:), allocatable :: C(:)

if(fallback)
  add_compile_options($<$<COMPILE_LANGUAGE:Fortran>:-Wno-unused-dummy-argument>)
  # spurious warning on fallback for stubs
endif()

endif()

# --- code coverage
if(ENABLE_COVERAGE)
include(CodeCoverage)
append_coverage_compiler_flags()
set(COVERAGE_EXCLUDES ${PROJECT_SOURCE_DIR}/src/tests)
endif()
