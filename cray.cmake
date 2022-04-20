# loads modules on Cray system and builds with CMake
# canNOT use from Project CMakeLists.txt
# it's OK to run again if you're not sure if it was already run.

cmake_minimum_required(VERSION 3.20...3.23)

if(NOT CMAKE_INSTALL_PREFIX)
  message(FATAL_ERROR "Please specify libraries install directory:
  cmake -DCMAKE_INSTALL_PREFIX=<install_dir> -P ${CMAKE_CURRENT_LIST_FILE}")
endif()

option(intel "use intel compiler instead of default GCC")

# the module commands only affect the current process, not the parent shell
if(intel)
  cmake_path(SET BINARY_DIR ${CMAKE_CURRENT_LIST_DIR}/build-intel)
  set(CXXFLAGS --gcc-toolchain=/opt/cray/pe/gcc/11.2.0/snos)
  # GCC >= 9.1 must be used for C++17 filesystem
else()
  cmake_path(SET BINARY_DIR ${CMAKE_CURRENT_LIST_DIR}/build)
endif()

find_package(EnvModules REQUIRED)

env_module(load cpe/22.03 OUTPUT_VARIABLE out RESULT_VARIABLE ret)
if(ret)
  message(STATUS "load cpe/22.03 error ${ret}: ${out}")
endif()

if(intel)
  env_module(load intel-oneapi/2022.0.2 OUTPUT_VARIABLE out RESULT_VARIABLE ret)
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
  env_module(load cray-mpich OUTPUT_VARIABLE out RESULT_VARIABLE ret)
  if(ret)
    message(STATUS "load cray-mpich error ${ret}: ${out}")
  endif()
endif()

execute_process(
COMMAND ${CMAKE_COMMAND}
  -S${CMAKE_CURRENT_LIST_DIR}
  -B${BINARY_DIR}
  -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
  -DCMAKE_CXX_FLAGS=${CXXFLAGS}
COMMAND_ERROR_IS_FATAL ANY
)

execute_process(
COMMAND ${CMAKE_COMMAND} --build ${BINARY_DIR}
COMMAND_ERROR_IS_FATAL ANY
)
