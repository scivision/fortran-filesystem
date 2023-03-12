set_property(DIRECTORY PROPERTY LABELS c)

foreach(t IN ITEMS expanduser ifc nullptr reserved)
  add_executable(test_C_${t} test_${t}.c)
  target_link_libraries(test_C_${t} PRIVATE ffilesystem)
  add_test(NAME C_${t} COMMAND test_C_${t})
endforeach()

add_executable(test_binpath_c test_binpath.c)
target_link_libraries(test_binpath_c PRIVATE ffilesystem)
add_test(NAME C_binpath
COMMAND test_binpath_c $<BOOL:${BUILD_SHARED_LIBS}> $<TARGET_FILE_NAME:test_binpath_c>  $<TARGET_FILE_NAME:ffilesystem>
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
