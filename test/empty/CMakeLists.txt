if(HAVE_CXX_FILESYSTEM)

add_executable(test_cpp_empty test_empty.cpp)
target_link_libraries(test_cpp_empty PRIVATE ffilesystem)

add_test(NAME Cpp_empty COMMAND test_cpp_empty $<BOOL:${BUILD_SHARED_LIBS}>)
set_property(TEST Cpp_empty PROPERTY SKIP_REGULAR_EXPRESSION "not implemented;SIGABRT;[aA]bort")

endif()
