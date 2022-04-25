# loads modules on Cray system and builds with CMake
# canNOT use from Project CMakeLists.txt
# it's OK to run again if you're not sure if it was already run.
#
# NOTE: your system may have different versions/paths, treat this like a template.
#
# NOTE: to specify install directory, run like:
#   cmake -DCMAKE_INSTALL_PREFIX=<install_dir> -P cray.cmake

cmake_minimum_required(VERSION 3.20)

# --- user options

option(intel "use intel compiler instead of default GCC")

# --- default params

cmake_path(SET gcc_dir /opt/cray/pe/gcc/11.2.0/snos)
set(cpe cpe/22.03)
set(mpi cray-mpich)
set(oneapi intel-oneapi/2022.0.2)

# --- main script

# the module commands only affect the current process, not the parent shell
if(intel)
  cmake_path(SET BINARY_DIR ${CMAKE_CURRENT_LIST_DIR}/build-intel)
  if(NOT DEFINED ENV{CXXFLAGS})
    set(ENV{CXXFLAGS} --gcc-toolchain=${gcc_dir})
  endif()
  # GCC >= 9.1 must be used for C++17 filesystem
else()
  cmake_path(SET BINARY_DIR ${CMAKE_CURRENT_LIST_DIR}/build)
endif()

set(cmake_args)
if(CMAKE_INSTALL_PREFIX)
  list(APPEND cmake_args -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX})
endif()

# --- modules

find_package(EnvModules REQUIRED)

if(cpe)
  env_module(load ${cpe} OUTPUT_VARIABLE out RESULT_VARIABLE ret)
  if(ret)
    message(STATUS "load ${cpe} error ${ret}: ${out}")
  endif()
endif()

if(intel)
  env_module(load ${oneapi} OUTPUT_VARIABLE out RESULT_VARIABLE ret)
  env_module_swap(PrgEnv-cray PrgEnv-intel OUTPUT_VARIABLE out RESULT_VARIABLE ret)
else()
  env_module_swap(PrgEnv-cray PrgEnv-gnu OUTPUT_VARIABLE out RESULT_VARIABLE ret)
endif()
if(ret)
  message(STATUS "swap PrgEnv error ${ret}: ${out}")
endif()

env_module(load python OUTPUT_VARIABLE out RESULT_VARIABLE ret)

# too compiler specific
# env_module(load cray-hdf5 OUTPUT_VARIABLE out RESULT_VARIABLE ret)
# if(ret)
#   message(STATUS "load cray-hdf5 error ${ret}: ${out}")
# endif()

if(NOT intel)
  env_module(load ${mpi} OUTPUT_VARIABLE out RESULT_VARIABLE ret)
  if(ret)
    message(STATUS "load ${mpi} error ${ret}: ${out}")
  endif()
endif()

execute_process(
COMMAND ${CMAKE_COMMAND} -S${CMAKE_CURRENT_LIST_DIR} -B${BINARY_DIR} ${cmake_args}
COMMAND_ERROR_IS_FATAL ANY
)

execute_process(
COMMAND ${CMAKE_COMMAND} --build ${BINARY_DIR}
COMMAND_ERROR_IS_FATAL ANY
)
