add_executable(test_pathlib test_pathlib.f90)
target_link_libraries(test_pathlib PRIVATE pathlib)
target_compile_options(test_pathlib PRIVATE $<$<Fortran_COMPILER_ID:GNU>:-Wno-intrinsic-shadow>)

add_test(NAME pathlib COMMAND test_pathlib)
set_tests_properties(pathlib PROPERTIES TIMEOUT 10)

if(ENABLE_COVERAGE)
setup_target_for_coverage_gcovr_html(
NAME coverage
EXECUTABLE ${CMAKE_CTEST_COMMAND}
)
endif()
