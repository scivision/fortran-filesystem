# run like:
#
#  ctest -S memcheck.cmake
#
# optionally, tell path to memory checker like:
#
# ctest -DMEMCHECK_ROOT=/path/to/bin/valgrind -S memcheck.cmake

list(APPEND opts -DCMAKE_BUILD_TYPE=Debug)

set(CTEST_TEST_TIMEOUT 60)
# takes effect only if test property TIMEOUT is not set

if(NOT DEFINED CTEST_MEMORYCHECK_TYPE)
  foreach(c valgrind drmemory)
    find_program(${c}_exe NAMES ${c}
    HINTS ${MEMCHECK_ROOT}
    PATH_SUFFIXES bin64 bin
    )
    if(${c}_exe)
      set(CTEST_MEMORYCHECK_COMMAND ${${c}_exe})
      break()
    endif()
  endforeach()
endif()

if(CTEST_MEMORYCHECK_COMMAND MATCHES "drmemory")
  set(CTEST_MEMORYCHECK_TYPE "DrMemory")
  set(CTEST_MEMORYCHECK_COMMAND_OPTIONS "-light -count_leaks")
elseif(CTEST_MEMORYCHECK_COMMAND MATCHES "valgrind")
  set(CTEST_MEMORYCHECK_TYPE "Valgrind")
endif()

if(NOT CTEST_MEMORYCHECK_COMMAND)
  message(FATAL_ERROR "No memory checker found")
endif()

set(CTEST_SOURCE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
set(CTEST_BINARY_DIRECTORY ${CTEST_SOURCE_DIRECTORY}/build-${CTEST_MEMORYCHECK_TYPE})
set(CTEST_BUILD_CONFIGURATION Debug)

if(WIN32)
  set(CTEST_CMAKE_GENERATOR "MinGW Makefiles")
else()
  set(CTEST_CMAKE_GENERATOR "Unix Makefiles")
endif()
set(CTEST_BUILD_FLAGS -j)

message(STATUS "Checker ${CTEST_MEMORYCHECK_TYPE}: ${CTEST_MEMORYCHECK_COMMAND}")

ctest_start(Experimental)

ctest_configure(
OPTIONS "${opts}"
RETURN_VALUE ret
CAPTURE_CMAKE_ERROR err
)
if(NOT (ret EQUAL 0 AND err EQUAL 0))
  message(FATAL_ERROR "CMake configure failed:  ${ret}   ${err}")
endif()

ctest_build(
RETURN_VALUE ret
CAPTURE_CMAKE_ERROR err
)
if(NOT (ret EQUAL 0 AND err EQUAL 0))
  message(FATAL_ERROR "CMake build failed:  ${ret}   ${err}")
endif()

ctest_memcheck(
RETURN_VALUE ret
CAPTURE_CMAKE_ERROR err
DEFECT_COUNT count
)

if(NOT (ret EQUAL 0 AND err EQUAL 0))
  message(FATAL_ERROR "Memory check failed:  ${ret}   ${err}")
endif()

if(NOT count EQUAL 0)
  message(FATAL_ERROR "Memory check found ${count} defects")
endif()
