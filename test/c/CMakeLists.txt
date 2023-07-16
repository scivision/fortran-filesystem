# --- C
foreach(t IN ITEMS canonical expanduser file env empty reserved)
  add_executable(test_C_${t} test_${t}.c)
  target_link_libraries(test_C_${t} PRIVATE ffilesystem)
  add_test(NAME C_${t} COMMAND test_C_${t})
  set_property(TEST C_${t} PROPERTY LABELS "C")
endforeach()

add_executable(test_binpath_c test_binpath.c)
target_link_libraries(test_binpath_c PRIVATE ffilesystem)
add_test(NAME C_binpath
COMMAND test_binpath_c
  $<AND:$<BOOL:${BUILD_SHARED_LIBS}>,$<OR:$<BOOL:${WIN32}>,$<BOOL:${CYGWIN}>,$<BOOL:${HAVE_DLADDR}>>>
  $<TARGET_FILE_BASE_NAME:test_binpath_c>
  $<TARGET_FILE_NAME:ffilesystem>
)

# --- C++
if(HAVE_CXX_FILESYSTEM)

foreach(t IN ITEMS core exe env)
  add_executable(test_Cpp_${t} test_${t}.cpp)
  target_link_libraries(test_Cpp_${t} PRIVATE ffilesystem)
  add_test(NAME Cpp_${t} COMMAND test_Cpp_${t})
  set_property(TEST Cpp_${t} PROPERTY LABELS "Cpp")
endforeach()


add_executable(test_binpath_cpp test_binpath.cpp)
target_link_libraries(test_binpath_cpp PRIVATE ffilesystem)
add_test(NAME Cpp_binpath
COMMAND test_binpath_cpp
  $<AND:$<BOOL:${BUILD_SHARED_LIBS}>,$<OR:$<BOOL:${WIN32}>,$<BOOL:${CYGWIN}>,$<BOOL:${HAVE_DLADDR}>>>
  $<TARGET_FILE_BASE_NAME:test_binpath_cpp>
  $<TARGET_FILE_NAME:ffilesystem>
)


endif()


# --- props

get_property(test_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY TESTS)
set_property(TEST ${test_names} PROPERTY RUN_SERIAL true)
# tests run much faster in serial, especially on Windows

# --- Windows shared DLLs
if((CYGWIN OR WIN32) AND CMAKE_VERSION VERSION_GREATER_EQUAL 3.22)
  set_property(TEST ${test_names} PROPERTY ENVIRONMENT_MODIFICATION "PATH=path_list_append:$<TARGET_FILE_DIR:ffilesystem>")
endif()
