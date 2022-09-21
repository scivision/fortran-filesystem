set_property(DIRECTORY PROPERTY LABELS cpp)

# --- C  interface
add_executable(test_c_ifc test_ifc.c)
target_link_libraries(test_c_ifc PRIVATE filesystem)
add_test(NAME C_interface COMMAND test_c_ifc)

# --- C++ interface
if(HAVE_CXX_FILESYSTEM)
  add_subdirectory(cpp)
endif()

if(fortran)
  add_subdirectory(fortran)
endif()

# --- props

get_property(test_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY TESTS)
set_property(TEST ${test_names} PROPERTY RUN_SERIAL true)
set_property(TEST ${test_names} PROPERTY TIMEOUT 15)
# tests run much faster in serial, especially on Windows

# --- coverage

if(coverage)
  setup_target_for_coverage_gcovr_html(
  NAME coverage
  EXECUTABLE ${CMAKE_CTEST_COMMAND}
  )
endif()

# --- Windows shared DLLs
if(WIN32 AND CMAKE_VERSION VERSION_GREATER_EQUAL 3.22)
  set_property(TEST ${test_names} PROPERTY ENVIRONMENT_MODIFICATION PATH=path_list_append:${PROJECT_BINARY_DIR})
endif()
