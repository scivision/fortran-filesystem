# --- base class
add_executable(test_pathlib test_pathlib.f90)
target_link_libraries(test_pathlib PRIVATE pathlib)
add_test(NAME pathlib COMMAND test_pathlib)

# --- C++17 filesystem methods
add_executable(test_cpp_fs test_cpp_fs.f90)
target_link_libraries(test_cpp_fs PRIVATE pathlib)
add_test(NAME CPP_filesystem COMMAND test_cpp_fs)

# --- relative_to
if(NOT HAVE_CXX17_FILESYSTEM)
  add_executable(test_relative_to test_relative.f90)
  target_link_libraries(test_relative_to PRIVATE pathlib)
  add_test(NAME relative_to COMMAND test_relative_to)
  set_tests_properties(relative_to PROPERTIES LINKER_LANGUAGE Fortran)
endif()

# --- file parts
add_executable(test_pathlib_parts test_parts.f90)
target_link_libraries(test_pathlib_parts PRIVATE pathlib)
add_test(NAME FileParts COMMAND test_pathlib_parts)

# --- expanduser
add_executable(test_expanduser test_expanduser.f90)
target_link_libraries(test_expanduser PRIVATE pathlib)
add_test(NAME Expanduser COMMAND test_expanduser)

# -- symlink
add_executable(test_symlink test_symlink.f90)
target_link_libraries(test_symlink PRIVATE pathlib)
add_test(NAME symlink COMMAND test_symlink)

# --- canonical
add_executable(test_canonical test_canonical.f90)
target_link_libraries(test_canonical PRIVATE pathlib)
add_test(NAME Canonical COMMAND test_canonical)

# --- file size
add_executable(test_filesize test_filesize.f90)
target_link_libraries(test_filesize PRIVATE pathlib)
add_test(NAME FileSize COMMAND test_filesize)

# --- exe detect
add_executable(test_exe test_exe.f90)
target_link_libraries(test_exe PRIVATE pathlib)
add_test(NAME ExeDetect COMMAND test_exe)

# --- file ops
add_executable(test_fileops test_fileop.f90)
target_link_libraries(test_fileops PRIVATE pathlib)

add_test(NAME FileOps
COMMAND test_fileops
WORKING_DIRECTORY $<TARGET_FILE_DIR:test_fileops>
)
# workdir avoids test state being shared between tests

# --- read/write text file
add_executable(test_rw_text test_rw_text.f90)
target_link_libraries(test_rw_text PRIVATE pathlib)
add_test(NAME ReadWriteText
COMMAND test_rw_text ${CMAKE_CURRENT_BINARY_DIR}/test_rw_text.txt
)

# --- find
add_executable(test_find test_find.f90)
target_link_libraries(test_find PRIVATE pathlib)
add_test(NAME Find COMMAND test_find)

# --- props
set_tests_properties(CPP_filesystem ExeDetect PROPERTIES
DISABLED $<NOT:$<BOOL:${HAVE_CXX17_FILESYSTEM}>>
)


set_target_properties(test_pathlib test_cpp_fs test_find test_rw_text test_filesize
test_fileops test_exe test_symlink test_expanduser test_canonical test_pathlib_parts
PROPERTIES
LINKER_LANGUAGE Fortran
)
# for Intel compiler

set_tests_properties(pathlib Canonical FileOps FileParts Find ExeDetect FileSize Expanduser ReadWriteText symlink
PROPERTIES
TIMEOUT 20
RUN_SERIAL true  # tests run much faster, especially on Windows
)

# --- coverage

if(ENABLE_COVERAGE)
setup_target_for_coverage_gcovr_html(
NAME coverage
EXECUTABLE ${CMAKE_CTEST_COMMAND}
)
endif()
