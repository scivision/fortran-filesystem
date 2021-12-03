add_executable(test_pathlib test_pathlib.f90)
target_link_libraries(test_pathlib PRIVATE pathlib)
target_compile_options(test_pathlib PRIVATE $<$<Fortran_COMPILER_ID:GNU>:-Wno-intrinsic-shadow>)

add_test(NAME pathlib COMMAND test_pathlib)
set_tests_properties(pathlib PROPERTIES TIMEOUT 10)


add_executable(test_canonical test_canonical.f90)
target_link_libraries(test_canonical PRIVATE pathlib)

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


if(ENABLE_COVERAGE)
setup_target_for_coverage_gcovr_html(
NAME coverage
EXECUTABLE ${CMAKE_CTEST_COMMAND}
)
endif()
