include(CheckFunctionExists)
include(CheckCXXSymbolExists)
include(CheckCXXSourceCompiles)

include(${CMAKE_CURRENT_LIST_DIR}/CppCheck.cmake)

# --- abi check: C++ and Fortran compiler ABI compatibility

if(NOT abi_ok)
  message(CHECK_START "checking that compilers can link together")
  try_compile(abi_ok
  ${CMAKE_CURRENT_BINARY_DIR}/abi_check ${CMAKE_CURRENT_LIST_DIR}/abi_check
  abi_check
  CMAKE_FLAGS -Dcpp:BOOL=${cpp} -Dfortran:BOOL=${fortran}
  )
  if(abi_ok)
    message(CHECK_PASS "OK")
  else()
    message(FATAL_ERROR "ABI-incompatible compilers:
    C compiler ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}
    C++ compiler ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}
    Fortran compiler ${CMAKE_Fortran_COMPILER_ID} ${CMAKE_Fortran_COMPILER_VERSION}"
    )
  endif()
endif()

#--- is dladdr available for lib_path() optional function
unset(CMAKE_REQUIRED_FLAGS)
if(BUILD_SHARED_LIBS AND NOT WIN32)
  set(CMAKE_REQUIRED_LIBRARIES ${CMAKE_DL_LIBS})
  check_function_exists(dladdr HAVE_DLADDR)
else()
  unset(HAVE_DLADDR CACHE)
endif()

# --- some compilers require these manual settings
unset(CMAKE_REQUIRED_LIBRARIES)
unset(CMAKE_REQUIRED_DEFINITIONS)

if((CMAKE_C_COMPILER_ID STREQUAL "GNU" AND CMAKE_C_COMPILER_VERSION VERSION_LESS "9.1.0") OR
   (CMAKE_Fortran_COMPILER_ID STREQUAL "GNU" AND CMAKE_Fortran_COMPILER_VERSION VERSION_LESS "9.1.0"))
  set(GNU_stdfs stdc++fs stdc++)
endif()
# need -lstdc++ to avoid C main program link error

if(CMAKE_C_COMPILER_ID STREQUAL "NVHPC")
  set(GNU_stdfs stdc++fs stdc++)
endif()

if(GNU_stdfs)
  set(CMAKE_REQUIRED_LIBRARIES ${GNU_stdfs})
  message(STATUS "adding library ${GNU_stdfs}")
endif()

if(cpp)
  cpp_check()
else()
  unset(HAVE_CXX_FILESYSTEM CACHE)
endif()


# --- deeper filesystem check: C, C++ and Fortran compiler ABI compatibility

if(HAVE_CXX_FILESYSTEM AND NOT DEFINED fs_abi_ok)
  message(CHECK_START "checking that compilers can link C++ Filesystem together")
  try_compile(fs_abi_ok
  ${CMAKE_CURRENT_BINARY_DIR}/fs_check ${CMAKE_CURRENT_LIST_DIR}/fs_check
  fs_check
  CMAKE_FLAGS -DGNU_stdfs=${GNU_stdfs} -Dfortran:BOOL=${fortran}
  )
  if(fs_abi_ok)
    message(CHECK_PASS "OK")
  else()
    message(WARNING "
    Disabling C++ filesystem due to ABI-incompatible compilers:
    C compiler ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}
    C++ compiler ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}
    Fortran compiler ${CMAKE_Fortran_COMPILER_ID} ${CMAKE_Fortran_COMPILER_VERSION}"
    )
    set(HAVE_CXX_FILESYSTEM false CACHE BOOL "ABI problem with C++ filesystem" FORCE)
  endif()
endif()

if(cpp AND NOT fallback AND NOT HAVE_CXX_FILESYSTEM)
  message(FATAL_ERROR "C++ filesystem not available. To fallback to C filesystem, set:
  cmake -Dfallback=on"
  )
endif()

# fixes errors about needing -fPIE
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(CMAKE_POSITION_INDEPENDENT_CODE true)
endif()

# --- C compile flags
if(CMAKE_C_COMPILER_ID MATCHES "Clang|GNU|^Intel")
  add_compile_options(
  "$<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:Debug>>:-Wextra>"
  "$<$<COMPILE_LANGUAGE:C>:-Wall>"
  )
  if(cpp)
    add_compile_options(
    "$<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:Debug>>:-Wextra>"
    "$<$<COMPILE_LANGUAGE:CXX>:-Wall>"
    )
  endif()
elseif(CMAKE_C_COMPILER_ID MATCHES "MSVC")
  add_compile_options("$<$<COMPILE_LANGUAGE:C>:/W3;/wd4996>")
  if(cpp)
    add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:/W3;/wd4996>")
  endif()
endif()

# --- Fortran compile flags
if(CMAKE_Fortran_COMPILER_ID MATCHES "^Intel")

add_compile_options(
"$<$<COMPILE_LANGUAGE:Fortran>:-warn>"
"$<$<AND:$<COMPILE_LANGUAGE:Fortran>,$<CONFIG:Debug>>:-traceback;-check;-debug>"
)

if(WIN32)
  add_compile_options($<$<AND:$<COMPILE_LANGUAGE:Fortran>,$<CONFIG:Debug>>:/Od>)
else()
  add_compile_options($<$<AND:$<COMPILE_LANGUAGE:Fortran>,$<CONFIG:Debug>>:-O0>)
endif()

elseif(CMAKE_Fortran_COMPILER_ID STREQUAL "GNU")

add_compile_options(
"$<$<AND:$<COMPILE_LANGUAGE:Fortran>,$<CONFIG:Debug>>:-Wextra>"
"$<$<COMPILE_LANGUAGE:Fortran>:-Wall;-fimplicit-none>"
"$<$<AND:$<COMPILE_LANGUAGE:Fortran>,$<CONFIG:Debug>>:-fcheck=all;-Werror=array-bounds>"
"$<$<AND:$<COMPILE_LANGUAGE:Fortran>,$<NOT:$<CONFIG:Debug>>>:-fno-backtrace>"
)

endif()

# --- code coverage
if(coverage)
  include(CodeCoverage)
  append_coverage_compiler_flags()
  set(COVERAGE_EXCLUDES ${PROJECT_SOURCE_DIR}/src/tests)
endif()

# --- clang-tidy
if(tidy)
  find_program(CLANG_TIDY_EXE NAMES clang-tidy REQUIRED
  PATHS /opt/homebrew/opt/llvm/bin
  )
  set(tidy_cmd ${CLANG_TIDY_EXE} -format-style=file)
  set(CMAKE_C_CLANG_TIDY ${tidy_cmd})
  set(CMAKE_CXX_CLANG_TIDY ${tidy_cmd})
endif()
