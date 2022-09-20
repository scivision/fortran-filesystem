set_property(DIRECTORY PROPERTY LABELS fortran)

# --- base class
add_executable(test_core test_core.f90)
target_link_libraries(test_core PRIVATE filesystem)
add_test(NAME filesystem_core COMMAND test_core)
set_property(TEST filesystem_core PROPERTY FIXTURES_SETUP core_fxt)

# --- C++ filesystem methods
add_executable(test_env test_cpp_fs.f90)
target_link_libraries(test_env PRIVATE filesystem)
add_test(NAME environment COMMAND test_env)

# --- relative_to
add_executable(test_relative_to test_cpp_relative.f90)
target_link_libraries(test_relative_to PRIVATE filesystem)
add_test(NAME relative_to COMMAND test_relative_to)
set_property(TEST relative_to PROPERTY DISABLED $<NOT:$<BOOL:${cpp}>>)
if(HAVE_CXX_FILESYSTEM)
  set_property(TEST relative_to PROPERTY PASS_REGULAR_EXPRESSION "OK: relative_to full")
endif()

# --- regex
add_executable(test_match test_match.f90)
target_link_libraries(test_match PRIVATE filesystem)
add_test(NAME match COMMAND test_match)
set_property(TEST match PROPERTY DISABLED $<NOT:$<BOOL:${cpp}>>)

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
  set_property(TEST Canonical PROPERTY PASS_REGULAR_EXPRESSION "OK: canonical full")
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

add_test(NAME FileOps COMMAND test_fileops)
set_property(TEST FileOps PROPERTY WORKING_DIRECTORY $<TARGET_FILE_DIR:test_fileops>)
set_property(TEST FileOps PROPERTY FIXTURES_SETUP ops_fxt)

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
set_property(TEST Find PROPERTY FIXTURES_REQUIRED ops_fxt)

# --- binpath
add_executable(test_binpath test_binpath.f90)
target_link_libraries(test_binpath PRIVATE filesystem)
add_test(NAME Binpath COMMAND test_binpath $<BOOL:${BUILD_SHARED_LIBS}>)


# --- props
set_property(TEST relative_to Canonical FileSize PROPERTY FIXTURES_REQUIRED core_fxt)

get_property(test_targets DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)
foreach(t IN LISTS test_targets)
  # Intel needs linker_language fortran else error "undefined reference to `main'"
  set_property(TARGET ${t} PROPERTY LINKER_LANGUAGE Fortran)
endforeach()

get_property(test_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY TESTS)
set_property(TEST ${test_names} PROPERTY RUN_SERIAL true)
set_property(TEST ${test_names} PROPERTY TIMEOUT 15)
# tests run much faster in serial, especially on Windows

# --- Windows shared DLLs
if(WIN32 AND CMAKE_VERSION VERSION_GREATER_EQUAL 3.22)
  set_property(TEST ${test_names} PROPERTY ENVIRONMENT_MODIFICATION PATH=path_list_append:${PROJECT_BINARY_DIR})
endif()
