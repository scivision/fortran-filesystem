cmake_minimum_required(VERSION 3.15...3.22)

project(FortranPathlibConcepts
LANGUAGES C Fortran
)

enable_testing()

add_library(canonical
$<IF:$<BOOL:${WIN32}>,canonical_windows.f90,canonical_posix.f90>
)

add_executable(test_canonical test_canonical.f90)
target_link_libraries(test_canonical PRIVATE canonical)

set(regex "OK: current dir = ${CMAKE_CURRENT_BINARY_DIR}")
if(WIN32)
  string(REPLACE "/" "\\\\" regex "${regex}")
endif()

add_test(NAME Canonical
COMMAND test_canonical
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
set_tests_properties(Canonical PROPERTIES
TIMEOUT 10
PASS_REGULAR_EXPRESSION ${regex}
)

message(STATUS "${CMAKE_CURRENT_BINARY_DIR}")
