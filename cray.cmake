# loads modules on Cray system
# canNOT use from Project CMakeLists.txt
# it's OK to run again if you're not sure if it was already run.
#
# NOTE: your Cray system may have different versions/paths, treat this like a template.
#
# NOTE: to specify install directory, run like:
#   cmake -DCMAKE_INSTALL_PREFIX=<install_dir> -P cray.cmake

cmake_minimum_required(VERSION 3.20)

# --- user options

option(intel "use intel compiler instead of default GCC")

# --- module names (may be different on your system)

set(gcc_mod gcc)
set(pecray PrgEnv-cray)
set(pegnu PrgEnv-gnu)
set(peintel PrgEnv-intel)

# --- main script

# propagate options
if(intel)
  set(ENV{_toolintel} ${intel})
else()
  set(intel "$ENV{_toolintel}")
endif()

find_package(EnvModules REQUIRED)

function(gcc_toolchain)

set(CXXFLAGS $ENV{CXXFLAGS})
if(CXXFLAGS MATCHES "--gcc-toolchain")
  return()
endif()

env_module(load ${gcc_mod} OUTPUT_VARIABLE out RESULT_VARIABLE ret)
message(STATUS "load ${gcc_mod}:    ${out}  ${ret}")

find_program(cc NAMES gcc REQUIRED)

execute_process(COMMAND ${cc} -dumpversion
OUTPUT_VARIABLE gcc_vers
COMMAND_ERROR_IS_FATAL ANY
)
if(gcc_vers VERSION_LESS 9.1)
  message(FATAL_ERROR "GCC toolchain >= 9.1 is required for oneAPI")
endif()

execute_process(COMMAND ${cc} -v
OUTPUT_VARIABLE gcc_verb
ERROR_VARIABLE gcc_verb
COMMAND_ERROR_IS_FATAL ANY
)

set(pat "--prefix=([/a-zA-Z0-9_\\-\\.]+)")
string(REGEX MATCH "${pat}" gcc_prefix "${gcc_verb}")

if(CMAKE_MATCH_1)
  string(APPEND CXXFLAGS " --gcc-toolchain=${CMAKE_MATCH_1}")
  set(ENV{CXXFLAGS} ${CXXFLAGS})
else()
  message(WARNING "GCC toolchain not found")
endif()

endfunction(gcc_toolchain)

# the module commands only affect the current process, not the parent shell
env_module_list(mods)

if(intel)
  cmake_path(SET BINARY_DIR ${CMAKE_CURRENT_LIST_DIR}/build-intel)

  if(NOT mods MATCHES "${peintel}")
    message(FATAL_ERROR "Cray Intel programming environment ${peintel} isn't loaded.")
  endif()
  # need new enough GCC toolchain
  gcc_toolchain()
else()
  cmake_path(SET BINARY_DIR ${CMAKE_CURRENT_LIST_DIR}/build)

  if(NOT mods MATCHES "${pegnu}")
    message(FATAL_ERROR "Cray GCC programming environment ${pegnu} isn't loaded.")
  endif()
endif()
