include(CheckFunctionExists)
include(CheckCXXSymbolExists)
include(CheckCXXSourceCompiles)

include(${CMAKE_CURRENT_LIST_DIR}/CppCheck.cmake)

# --- abi check: C++ and Fortran compiler ABI compatibility

if(cpp AND fortran AND NOT abi_ok)
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

# --- some compilers require these manual settings
unset(CMAKE_REQUIRED_LIBRARIES)
unset(CMAKE_REQUIRED_DEFINITIONS)

if((CMAKE_C_COMPILER_ID STREQUAL "GNU" AND CMAKE_C_COMPILER_VERSION VERSION_LESS "9.1.0") OR
    CMAKE_C_COMPILER_ID STREQUAL "NVHPC")
  set(NEED_stdfs stdc++fs)
  set(CMAKE_REQUIRED_LIBRARIES ${NEED_stdfs})
  message(STATUS "adding library ${NEED_stdfs} for ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPLIER_VERSION}")
endif()

if(MSVC)
  set(CMAKE_REQUIRED_FLAGS /std:c++17)
else()
  set(CMAKE_REQUIRED_FLAGS -std=c++17)
endif()

if(cpp)
  cpp_check()
else()
  unset(HAVE_CXX_FILESYSTEM CACHE)
endif()

#--- is dladdr available for lib_path() optional function
if(NOT WIN32)
  set(CMAKE_REQUIRED_LIBRARIES ${CMAKE_DL_LIBS})
  set(CMAKE_REQUIRED_DEFINITIONS -D_GNU_SOURCE)
  check_function_exists(dladdr HAVE_DLADDR)
endif()

# fixes errors about needing -fPIE
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(CMAKE_POSITION_INDEPENDENT_CODE true)
endif()

# --- compile flags

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  add_compile_options($<$<COMPILE_LANGUAGE:C>:-Werror=implicit-function-declaration>
  )
  # "$<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:Debug>>:-fsanitize=address>"
elseif(CMAKE_C_COMPILER_ID MATCHES "(Clang|Intel)")
  add_compile_options(
  "$<$<COMPILE_LANGUAGE:C,CXX>:-Wall;-Wextra>"
  "$<$<COMPILE_LANGUAGE:C>:-Werror=implicit-function-declaration>"
  )
elseif(CMAKE_C_COMPILER_ID MATCHES "MSVC")
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

if(NOT cpp)
  add_compile_options($<$<COMPILE_LANGUAGE:Fortran>:-Wno-unused-dummy-argument>)
  # spurious warning for C stubs
endif()

endif()

# --- code coverage
if(coverage)
  include(CodeCoverage)
  append_coverage_compiler_flags()
  set(COVERAGE_EXCLUDES ${PROJECT_SOURCE_DIR}/src/tests)
endif()
