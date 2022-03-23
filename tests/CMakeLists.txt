cmake_minimum_required(VERSION 3.21...3.22)

set_directory_properties(PROPERTIES LABELS unit)

# --- base class
add_executable(test_filesystem test_filesystem.f90)
target_link_libraries(test_filesystem PRIVATE filesystem)
add_test(NAME filesystem COMMAND test_filesystem)

# --- C++17 filesystem methods
add_executable(test_cpp_fs test_cpp_fs.f90)
target_link_libraries(test_cpp_fs PRIVATE filesystem)
add_test(NAME CPP_filesystem COMMAND test_cpp_fs)

# --- relative_to
add_executable(test_relative_to test_cpp_relative.f90)
target_link_libraries(test_relative_to PRIVATE filesystem)
add_test(NAME relative_to COMMAND test_relative_to)
if(HAVE_CXX_FILESYSTEM)
  set_tests_properties(relative_to PROPERTIES PASS_REGULAR_EXPRESSION "OK: relative_to full")
endif()

# --- file parts
add_executable(test_filesystem_parts test_parts.f90)
target_link_libraries(test_filesystem_parts PRIVATE filesystem)
add_test(NAME FileParts COMMAND test_filesystem_parts)

# --- expanduser
add_executable(test_expanduser test_expanduser.f90)
target_link_libraries(test_expanduser PRIVATE filesystem)
add_test(NAME Expanduser COMMAND test_expanduser)

# -- symlink
add_executable(test_symlink test_symlink.f90)
target_link_libraries(test_symlink PRIVATE filesystem)
add_test(NAME symlink COMMAND test_symlink)

# --- canonical
add_executable(test_canonical test_canonical.f90)
target_link_libraries(test_canonical PRIVATE filesystem)
add_test(NAME Canonical COMMAND test_canonical)
if(HAVE_CXX_FILESYSTEM)
  set_tests_properties(Canonical PROPERTIES PASS_REGULAR_EXPRESSION "OK: canonical full")
endif()

# --- file size
add_executable(test_filesize test_filesize.f90)
target_link_libraries(test_filesize PRIVATE filesystem)
add_test(NAME FileSize COMMAND test_filesize)

# --- exe detect
add_executable(test_exe test_exe.f90)
target_link_libraries(test_exe PRIVATE filesystem)
add_test(NAME ExeDetect COMMAND test_exe)

# --- file ops
add_executable(test_fileops test_fileop.f90)
target_link_libraries(test_fileops PRIVATE filesystem)

add_test(NAME FileOps
COMMAND test_fileops
WORKING_DIRECTORY $<TARGET_FILE_DIR:test_fileops>
)
# workdir avoids test state being shared between tests

# --- read/write text file
add_executable(test_rw_text test_rw_text.f90)
target_link_libraries(test_rw_text PRIVATE filesystem)
add_test(NAME ReadWriteText
COMMAND test_rw_text ${CMAKE_CURRENT_BINARY_DIR}/test_rw_text.txt
)

# --- find
add_executable(test_find test_find.f90)
target_link_libraries(test_find PRIVATE filesystem)
add_test(NAME Find COMMAND test_find)

# --- props

set_target_properties(test_filesystem test_cpp_fs test_find test_rw_text test_filesize
test_fileops test_exe test_symlink test_expanduser test_canonical test_filesystem_parts
test_relative_to
PROPERTIES
LINKER_LANGUAGE Fortran
)
# for Intel compiler

set_tests_properties(filesystem Canonical FileOps FileParts Find ExeDetect FileSize Expanduser ReadWriteText symlink
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

# --- Windows shared DLLs
if(WIN32 AND BUILD_SHARED_LIBS)
  add_custom_command(TARGET test_filesystem POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:test_filesystem> $<TARGET_FILE_DIR:test_filesystem>
  COMMAND_EXPAND_LISTS
  )
endif()
