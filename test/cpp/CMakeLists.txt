set_property(DIRECTORY PROPERTY LABELS cpp)

add_executable(test_cpp_ifc test_ifc.cpp)
target_link_libraries(test_cpp_ifc PRIVATE ffilesystem)
add_test(NAME CPP_interface COMMAND test_cpp_ifc)

add_executable(test_binpath_cpp test_binpath.cpp)
target_link_libraries(test_binpath_cpp PRIVATE ffilesystem)
add_test(NAME binpath_cpp
COMMAND test_binpath_cpp $<BOOL:${BUILD_SHARED_LIBS}> $<TARGET_FILE_NAME:test_binpath_cpp>  $<TARGET_FILE_NAME:ffilesystem>
)


# --- props
get_property(test_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY TESTS)
set_property(TEST ${test_names} PROPERTY RUN_SERIAL true)
# tests run much faster in serial, especially on Windows

# --- Windows shared DLLs
if(WIN32 AND CMAKE_VERSION VERSION_GREATER_EQUAL 3.22)
  dll_test_path(filesystem "${test_names}")
  set_property(TEST ${test_names} PROPERTY ENVIRONMENT_MODIFICATION "PATH=path_list_append:$<TARGET_FILE_DIR:ffilesystem>")
endif()
