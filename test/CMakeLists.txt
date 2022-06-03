set_directory_properties(PROPERTIES LABELS unit)

# --- base class
add_executable(test_core test_core.f90)
target_link_libraries(test_core PRIVATE filesystem)
add_test(NAME filesystem_core COMMAND test_core)
set_tests_properties(filesystem_core PROPERTIES
FIXTURES_SETUP core_fxt
)

# --- C++ filesystem methods
add_executable(test_env test_cpp_fs.f90)
target_link_libraries(test_env PRIVATE filesystem)
add_test(NAME environment COMMAND test_env)

# --- relative_to
add_executable(test_relative_to test_cpp_relative.f90)
target_link_libraries(test_relative_to PRIVATE filesystem)
add_test(NAME relative_to COMMAND test_relative_to)
set_tests_properties(relative_to PROPERTIES
DISABLED $<BOOL:${fallback}>
)
if(HAVE_CXX_FILESYSTEM)
  set_tests_properties(relative_to PROPERTIES PASS_REGULAR_EXPRESSION "OK: relative_to full")
endif()

# --- regex
add_executable(test_match test_match.f90)
target_link_libraries(test_match PRIVATE filesystem)
add_test(NAME match COMMAND test_match)
set_tests_properties(match PROPERTIES DISABLED $<BOOL:${fallback}>)

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
set_tests_properties(symlink PROPERTIES
SKIP_RETURN_CODE 77
)

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
set_tests_properties(ExeDetect PROPERTIES DISABLED $<BOOL:${fallback}>)

# --- file ops
add_executable(test_fileops test_fileop.f90)
target_link_libraries(test_fileops PRIVATE filesystem)

add_test(NAME FileOps COMMAND test_fileops)
set_tests_properties(FileOps PROPERTIES
WORKING_DIRECTORY $<TARGET_FILE_DIR:test_fileops>
SKIP_RETURN_CODE 77
FIXTURES_SETUP ops_fxt
)

# workdir avoids test state being shared between tests

# --- read/write text file
add_executable(test_rw_text test_rw_text.f90)
target_link_libraries(test_rw_text PRIVATE filesystem)
add_test(NAME ReadWriteText
COMMAND test_rw_text ${CMAKE_CURRENT_BINARY_DIR}/test_rw_text.txt
)
set_tests_properties(ReadWriteText PROPERTIES DISABLED $<BOOL:${fallback}>)

# --- find
add_executable(test_find test_find.f90)
target_link_libraries(test_find PRIVATE filesystem)
add_test(NAME Find COMMAND test_find)
set_tests_properties(Find PROPERTIES
FIXTURES_REQUIRED ops_fxt
)

# --- C / C++ interface
add_executable(test_cpp_ifc test_ifc.cpp)
target_compile_features(test_cpp_ifc PRIVATE cxx_std_17)
target_link_libraries(test_cpp_ifc PRIVATE filesystem ${CMAKE_REQUIRED_LIBRARIES})
target_include_directories(test_cpp_ifc PRIVATE ${PROJECT_SOURCE_DIR}/include)
set_target_properties(test_cpp_ifc PROPERTIES
LABELS cpp
LINKER_LANGUAGE CXX
)
add_test(NAME CPP_interface COMMAND test_cpp_ifc)

add_executable(test_c_ifc test_ifc.c)
set_target_properties(test_c_ifc PROPERTIES
LABELS cpp
LINKER_LANGUAGE CXX
)
target_link_libraries(test_c_ifc PRIVATE filesystem ${CMAKE_REQUIRED_LIBRARIES})
target_include_directories(test_c_ifc PRIVATE ${PROJECT_SOURCE_DIR}/include)
add_test(NAME C_interface COMMAND test_c_ifc)

# --- props

set_tests_properties(relative_to Canonical FileSize PROPERTIES
FIXTURES_REQUIRED core_fxt
)

get_property(test_targets DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)
foreach(t IN LISTS test_targets)
  get_target_property(tlbl ${t} LABELS)
  if("cpp" IN_LIST tlbl)
    get_target_property(tlang ${t} LINKER_LANGUAGE)
    message(VERBOSE "link_language: ${tlang} ${t}")
    continue()
  endif()

  if(CMAKE_CXX_COMPILER_ID MATCHES "^Intel")
    # Intel needs linker_language fortran else error "undefined reference to `main'"
    set_target_properties(${t} PROPERTIES LINKER_LANGUAGE Fortran)
  else()
    # setting linker language ensures stdc++ is linked, else hundreds of linker errors
    set_target_properties(${t} PROPERTIES LINKER_LANGUAGE CXX)
  endif()
endforeach()

get_property(test_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY TESTS)
set_tests_properties(${test_names} PROPERTIES
RUN_SERIAL true
TIMEOUT 15
)
# tests run much faster in serial, especially on Windows

# --- coverage

if(ENABLE_COVERAGE)
setup_target_for_coverage_gcovr_html(
NAME coverage
EXECUTABLE ${CMAKE_CTEST_COMMAND}
)
endif()

# --- Windows shared DLLs
if(WIN32 AND BUILD_SHARED_LIBS)
  add_custom_command(TARGET test_core POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:test_core> $<TARGET_FILE_DIR:test_core>
  COMMAND_EXPAND_LISTS
  )
endif()
