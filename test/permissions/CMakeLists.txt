if(HAVE_CXX_FILESYSTEM)

add_executable(test_permissions_cpp test_permissions.cpp)
target_link_libraries(test_permissions_cpp PRIVATE ffilesystem)

add_test(NAME Cpp_permissions COMMAND test_permissions_cpp)

endif()



if(${PROJECT_NAME}_fortran)
add_executable(test_permissions_fortran test_permissions.f90)
target_link_libraries(test_permissions_fortran PRIVATE ffilesystem)
set_property(TARGET test_permissions_fortran PROPERTY LINKER_LANGUAGE Fortran)

add_test(NAME Fortran_permissions COMMAND test_permissions_fortran)
endif()

get_property(test_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY TESTS)

set_property(TEST ${test_names} PROPERTY SKIP_RETURN_CODE 77)
