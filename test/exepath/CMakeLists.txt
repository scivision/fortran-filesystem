if(HAVE_CXX_FILESYSTEM)

add_executable(test_exepath_cpp test_exepath.cpp)
target_link_libraries(test_exepath_cpp PRIVATE ffilesystem)

add_test(NAME Cpp_exepath COMMAND test_exepath_cpp test_exepath_cpp)

endif()


if(${PROJECT_NAME}_fortran)

add_executable(test_exepath_fortran test_exepath.f90)
target_link_libraries(test_exepath_fortran PRIVATE ffilesystem)
set_property(TARGET test_exepath_fortran PROPERTY LINKER_LANGUAGE Fortran)

add_test(NAME Fortran_exepath COMMAND test_exepath_fortran test_exepath_fortran)

endif()

get_property(test_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY TESTS)

set_property(TEST ${test_names} PROPERTY DISABLED $<NOT:$<BOOL:${HAVE_CXX_FILESYSTEM}>>)
