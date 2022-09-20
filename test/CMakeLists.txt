# --- C  interface
add_executable(test_c_ifc test_ifc.c)
set_property(TARGET test_c_ifc PROPERTY LABELS cpp)
target_link_libraries(test_c_ifc PRIVATE filesystem)
add_test(NAME C_interface COMMAND test_c_ifc)

# --- C++ interface
if(cpp)
  add_subdirectory(cpp)
endif()

if(fortran)
  add_subdirectory(fortran)
endif()

# --- props

get_property(test_targets DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)
foreach(t IN LISTS test_targets)
  # Intel needs linker_language fortran else error "undefined reference to `main'"
  set_property(TARGET ${t} PROPERTY LINKER_LANGUAGE Fortran)
endforeach()

get_property(test_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY TESTS)
set_property(TEST ${test_names} PROPERTY RUN_SERIAL true)
set_property(TEST ${test_names} PROPERTY TIMEOUT 15)
# tests run much faster in serial, especially on Windows

# --- coverage

if(ENABLE_COVERAGE)
setup_target_for_coverage_gcovr_html(
NAME coverage
EXECUTABLE ${CMAKE_CTEST_COMMAND}
)
endif()

# --- Windows shared DLLs
if(WIN32 AND CMAKE_VERSION VERSION_GREATER_EQUAL 3.22)
  set_property(TEST ${test_names} PROPERTY ENVIRONMENT_MODIFICATION PATH=path_list_append:${PROJECT_BINARY_DIR})
endif()
