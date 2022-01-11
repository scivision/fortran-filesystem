# --- base class
add_executable(test_pathlib test_pathlib.f90)
target_link_libraries(test_pathlib PRIVATE pathlib)
add_test(NAME pathlib COMMAND test_pathlib)

# --- file parts
add_executable(test_pathlib_parts test_parts.f90)
target_link_libraries(test_pathlib_parts PRIVATE pathlib)
add_test(NAME FileParts COMMAND test_pathlib_parts)

# --- expanduser
add_executable(test_expanduser test_expanduser.f90)
target_link_libraries(test_expanduser PRIVATE pathlib)
add_test(NAME Expanduser COMMAND test_expanduser)

# --- canonical
add_executable(test_canonical test_canonical.f90)
target_link_libraries(test_canonical PRIVATE pathlib)

add_test(NAME Canonical
COMMAND test_canonical
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# --- file size
add_executable(test_filesize test_filesize.f90)
target_link_libraries(test_filesize PRIVATE pathlib)

add_test(NAME FileSize COMMAND test_filesize WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# --- exe detect
# non-exe doesn't work on WSL due to filesystem interface
set(dummy_exe ${CMAKE_CURRENT_BINARY_DIR}/dummy.exe)
file(TOUCH ${dummy_exe})
file(CHMOD ${dummy_exe} PERMISSIONS OWNER_EXECUTE OWNER_READ GROUP_EXECUTE)
if(NOT CMAKE_HOST_SYSTEM_VERSION MATCHES "microsoft-standard")
  set(dummy_noexe ${CMAKE_CURRENT_BINARY_DIR}/dummy)
  file(TOUCH ${dummy_noexe})
endif()

add_executable(test_exe test_exe.f90)
target_link_libraries(test_exe PRIVATE pathlib)

add_test(NAME ExeDetect
COMMAND test_exe ${dummy_exe} ${dummy_noexe}
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)


# --- file ops
add_executable(test_fileops test_fileop.f90)
target_link_libraries(test_fileops PRIVATE pathlib)

add_test(NAME FileOps
COMMAND test_fileops
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# --- read/write text file
add_executable(test_rw_text test_rw_text.f90)
target_link_libraries(test_rw_text PRIVATE pathlib)

add_test(NAME ReadWriteText
COMMAND test_rw_text ${CMAKE_CURRENT_BINARY_DIR}/test_rw_text.txt)

# --- find
add_executable(test_find test_find.f90)
target_link_libraries(test_find PRIVATE pathlib)

add_test(NAME Find COMMAND test_find)

# --- props

set_tests_properties(pathlib Canonical FileOps FileParts Find ExeDetect FileSize Expanduser ReadWriteText
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
