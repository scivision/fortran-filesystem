set_directory_properties(PROPERTIES LABELS unit)

# --- base class
add_executable(test_core test_core.f90)
target_link_libraries(test_core PRIVATE filesystem)
add_test(NAME filesystem_core COMMAND test_core)

# --- C++ filesystem methods
add_executable(test_core_fs test_cpp_fs.f90)
target_link_libraries(test_core_fs PRIVATE filesystem)
add_test(NAME core COMMAND test_core_fs)

# --- relative_to
if(NOT fallback)
add_executable(test_relative_to test_cpp_relative.f90)
target_link_libraries(test_relative_to PRIVATE filesystem)
add_test(NAME relative_to COMMAND test_relative_to)
if(HAVE_CXX_FILESYSTEM)
  set_tests_properties(relative_to PROPERTIES PASS_REGULAR_EXPRESSION "OK: relative_to full")
endif()
endif()

# --- regex
if(NOT fallback)
add_executable(test_match test_match.f90)
target_link_libraries(test_match PRIVATE filesystem)
add_test(NAME match COMMAND test_match)
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
if(fallback AND CMAKE_Fortran_COMPILER_ID STREQUAL Cray)
  set_tests_properties(FileSize PROPERTIES DISABLED 1)
endif()

# --- exe detect
if(NOT fallback)
add_executable(test_exe test_exe.f90)
target_link_libraries(test_exe PRIVATE filesystem)
add_test(NAME ExeDetect COMMAND test_exe)
endif()

# --- file ops
add_executable(test_fileops test_fileop.f90)
target_link_libraries(test_fileops PRIVATE filesystem)

add_test(NAME FileOps
COMMAND test_fileops
WORKING_DIRECTORY $<TARGET_FILE_DIR:test_fileops>
)
# workdir avoids test state being shared between tests

# --- read/write text file
if(NOT fallback)
add_executable(test_rw_text test_rw_text.f90)
target_link_libraries(test_rw_text PRIVATE filesystem)
add_test(NAME ReadWriteText
COMMAND test_rw_text ${CMAKE_CURRENT_BINARY_DIR}/test_rw_text.txt
)
endif()

# --- find
add_executable(test_find test_find.f90)
target_link_libraries(test_find PRIVATE filesystem)
add_test(NAME Find COMMAND test_find)

# --- C / C++ interface
if(NOT fallback)
add_executable(test_cpp_ifc test_cpp_ifc.cpp)
target_compile_features(test_cpp_ifc PRIVATE cxx_std_17)
target_link_libraries(test_cpp_ifc PRIVATE filesystem ${CMAKE_REQUIRED_LIBRARIES})
target_include_directories(test_cpp_ifc PRIVATE ${PROJECT_SOURCE_DIR}/src)
set_target_properties(test_cpp_ifc PROPERTIES
LABELS cpp
LINKER_LANGUAGE CXX
)
add_test(NAME CPP_interface COMMAND test_cpp_ifc)

add_executable(test_c_ifc test_c_ifc.c)
set_target_properties(test_c_ifc PROPERTIES
LABELS cpp
LINKER_LANGUAGE CXX
)
target_link_libraries(test_c_ifc PRIVATE filesystem ${CMAKE_REQUIRED_LIBRARIES})
target_include_directories(test_c_ifc PRIVATE ${PROJECT_SOURCE_DIR}/src)
add_test(NAME C_interface COMMAND test_c_ifc)

endif(NOT fallback)



# --- props

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
set_tests_properties(${test_names} PROPERTIES RUN_SERIAL true)
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
