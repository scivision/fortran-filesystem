set_property(DIRECTORY PROPERTY LABELS cpp)


foreach(t IN ITEMS expanduser ifc nullptr)
  add_executable(test_CPP_${t} test_${t}.cpp)
  target_link_libraries(test_CPP_${t} PRIVATE ffilesystem)
  add_test(NAME CPP_${t} COMMAND test_CPP_${t})
endforeach()

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
