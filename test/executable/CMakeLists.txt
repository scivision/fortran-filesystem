if(HAVE_CXX_FILESYSTEM)

add_executable(test_exe_cpp test_exe.cpp)
target_link_libraries(test_exe_cpp PRIVATE ffilesystem)
add_test(NAME Cpp_exe COMMAND test_exe_cpp)

endif()


if(ffilesystem_fortran)

add_executable(test_exe_fortran test_exe.f90)
target_link_libraries(test_exe_fortran PRIVATE ffilesystem)
target_compile_options(test_exe_fortran PRIVATE ${${PROJECT_NAME}_fortran_test_flags})
set_property(TARGET test_exe_fortran PROPERTY LINKER_LANGUAGE Fortran)

add_test(NAME Fortran_exe COMMAND test_exe_fortran)

endif()


get_property(test_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY TESTS)

set_property(TEST ${test_names} PROPERTY SKIP_RETURN_CODE 77)
