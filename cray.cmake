# loads modules on Cray system and builds with CMake
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

set(gcc gcc)
set(oneapi intel-oneapi)
set(pecray PrgEnv-cray)
set(pegnu PrgEnv-gnu)
set(peintel PrgEnv-intel)

# --- main script

# the module commands only affect the current process, not the parent shell
if(intel)
  cmake_path(SET BINARY_DIR ${CMAKE_CURRENT_LIST_DIR}/build-intel)
  # if(NOT DEFINED ENV{CXXFLAGS})
  #   set(ENV{CXXFLAGS} --gcc-toolchain=${gcc_dir})
  # endif()
else()
  cmake_path(SET BINARY_DIR ${CMAKE_CURRENT_LIST_DIR}/build)
endif()

if(CMAKE_INSTALL_PREFIX)
  set(cmake_args -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX})
endif()

# --- modules

find_package(EnvModules REQUIRED)

if(intel)
  env_module(load ${gcc} OUTPUT_VARIABLE out RESULT_VARIABLE ret)
  if(ret)
    message(STATUS "load ${gcc} error ${ret}: ${out}")
  endif()

  execute_process(COMMAND gcc -dumpversion
  OUTPUT_VARIABLE gcc_vers
  COMMAND_ERROR_IS_FATAL ANY
  )
  if(gcc_vers VERSION_LESS 9.1)
    message(WARNING "GCC >= 9.1 is required")
  endif()

  env_module(load ${oneapi} OUTPUT_VARIABLE out RESULT_VARIABLE ret)
  message(STATUS "load ${oneapi}:  ${out}  ${ret}")

  env_module_swap(${pecray} ${peintel} OUTPUT_VARIABLE out RESULT_VARIABLE ret)
else()
  env_module_swap(${pecray} ${pegnu} OUTPUT_VARIABLE out RESULT_VARIABLE ret)
endif()
message(STATUS "swap PrgEnv: ${out}   ${ret}")

execute_process(
COMMAND ${CMAKE_COMMAND} -S${CMAKE_CURRENT_LIST_DIR} -B${BINARY_DIR} ${cmake_args}
COMMAND_ERROR_IS_FATAL ANY
)

execute_process(
COMMAND ${CMAKE_COMMAND} --build ${BINARY_DIR}
COMMAND_ERROR_IS_FATAL ANY
)

if(CMAKE_INSTALL_PREFIX)
  execute_process(
  COMMAND ${CMAKE_COMMAND} --install ${BINARY_DIR}
  COMMAND_ERROR_IS_FATAL ANY
  )
endif()
