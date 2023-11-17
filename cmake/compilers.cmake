include(CheckIncludeFile)
include(CheckSymbolExists)
include(CheckCXXSymbolExists)
include(CheckCXXSourceCompiles)

include(${CMAKE_CURRENT_LIST_DIR}/CppCheck.cmake)

check_include_file("sys/utsname.h" HAVE_UTSNAME_H)

#--- is dladdr available for lib_path() optional function
unset(CMAKE_REQUIRED_FLAGS)
if(BUILD_SHARED_LIBS AND NOT (WIN32 OR CYGWIN))
  set(CMAKE_REQUIRED_LIBRARIES ${CMAKE_DL_LIBS})
  if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CMAKE_REQUIRED_DEFINITIONS -D_GNU_SOURCE)
  endif()
  check_symbol_exists(dladdr "dlfcn.h" HAVE_DLADDR)
else()
  unset(HAVE_DLADDR CACHE)
endif()

# --- some compilers require these manual settings
unset(CMAKE_REQUIRED_LIBRARIES)
unset(CMAKE_REQUIRED_DEFINITIONS)
unset(GNU_stdfs)

if((CMAKE_C_COMPILER_ID STREQUAL "GNU" AND CMAKE_C_COMPILER_VERSION VERSION_LESS "9.1.0") OR
   (CMAKE_Fortran_COMPILER_ID STREQUAL "GNU" AND CMAKE_Fortran_COMPILER_VERSION VERSION_LESS "9.1.0"))
  set(GNU_stdfs stdc++fs stdc++)
endif()
# GCC < 9.1 needs -lstdc++ to avoid C main program link error
# NVHPC at least 23.11 and newer doesn't need the flags.

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
  message(FATAL_ERROR "C++ filesystem not available. To fallback to C filesystem:
  cmake -Dfallback=on -B build"
  )
endif()

# warn of shaky macOS compiler mix
set(ffilesystem_shaky false)
if(HAVE_CXX_FILESYSTEM AND APPLE)
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_Fortran_COMPILER_ID STREQUAL "GNU")
    set(ffilesystem_shaky true)
    message(WARNING "macOS Clang compiler with Gfortran may not catch C++ exceptions, which may halt the user program if a filesystem error occurs.")
  endif()
endif()

# fixes errors about needing -fPIE
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  include(CheckPIESupported)
  check_pie_supported()
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

if(CMAKE_C_COMPILER_ID STREQUAL "IntelLLVM")
  add_compile_options("$<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:Debug>>:-Rno-debug-disables-optimization>")
  if(cpp)
    add_compile_options("$<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:Debug>>:-Rno-debug-disables-optimization>")
  endif()
endif()

# --- Fortran compile flags
if(CMAKE_Fortran_COMPILER_ID MATCHES "^Intel")

add_compile_options(
"$<$<COMPILE_LANGUAGE:Fortran>:-warn>"
"$<$<AND:$<COMPILE_LANGUAGE:Fortran>,$<CONFIG:Debug>>:-traceback;-check;-debug>"
)

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

# --- IWYU
if(iwyu)
  find_program(IWYU_EXE NAMES include-what-you-use REQUIRED)
  message(STATUS "IWYU_EXE: ${IWYU_EXE}")
  set(iwyu_cmd ${IWYU_EXE})
  set(CMAKE_C_INCLUDE_WHAT_YOU_USE ${iwyu_cmd})
  set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_cmd})
endif()
