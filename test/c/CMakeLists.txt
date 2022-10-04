set_property(DIRECTORY PROPERTY LABELS c)

add_executable(test_c_ifc test_ifc.c)
target_link_libraries(test_c_ifc PRIVATE ffilesystem)
add_test(NAME C_interface COMMAND test_c_ifc)

add_executable(test_expanduser_c test_expanduser.c)
target_link_libraries(test_expanduser_c PRIVATE ffilesystem)
add_test(NAME C_expanduser COMMAND test_expanduser_c)

add_executable(test_binpath_c test_binpath.c)
target_link_libraries(test_binpath_c PRIVATE ffilesystem)
add_test(NAME binpath_c COMMAND test_binpath_c $<BOOL:${BUILD_SHARED_LIBS}>)

# --- props

get_property(test_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY TESTS)
set_property(TEST ${test_names} PROPERTY RUN_SERIAL true)
# tests run much faster in serial, especially on Windows

# --- Windows shared DLLs
if(WIN32 AND CMAKE_VERSION VERSION_GREATER_EQUAL 3.22)
  set_property(TEST ${test_names} PROPERTY ENVIRONMENT_MODIFICATION PATH=path_list_append:${PROJECT_BINARY_DIR})
endif()
