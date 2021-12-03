add_executable(test_pathlib test_pathlib.f90)
target_link_libraries(test_pathlib PRIVATE pathlib)
target_compile_options(test_pathlib PRIVATE $<$<Fortran_COMPILER_ID:GNU>:-Wno-intrinsic-shadow>)

set(dummy ${CMAKE_CURRENT_BINARY_DIR}/dummy)
set(dummy_exe ${CMAKE_CURRENT_BINARY_DIR}/dummy.exe)
file(TOUCH ${dummy_exe})
file(TOUCH ${dummy})
file(CHMOD ${dummy_exe} PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE)

add_test(NAME pathlib
COMMAND test_pathlib ${dummy_exe} ${dummy}
)
set_tests_properties(pathlib PROPERTIES TIMEOUT 10)


add_executable(test_canonical test_canonical.f90)
target_link_libraries(test_canonical PRIVATE pathlib)

add_test(NAME Canonical
COMMAND test_canonical
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
set_tests_properties(Canonical PROPERTIES
TIMEOUT 10
)


if(ENABLE_COVERAGE)
setup_target_for_coverage_gcovr_html(
NAME coverage
EXECUTABLE ${CMAKE_CTEST_COMMAND}
)
endif()
