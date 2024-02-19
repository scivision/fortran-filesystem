add_executable(test_reserved_c test_reserved.c)
target_link_libraries(test_reserved_c PRIVATE ffilesystem)
add_test(NAME C_reserved COMMAND test_reserved_c)

if(ffilesystem_fortran)
add_executable(test_reserved_fortran test_reserved.f90)
target_link_libraries(test_reserved_fortran PRIVATE ffilesystem)
set_property(TARGET test_reserved_fortran PROPERTY LINKER_LANGUAGE Fortran)

add_test(NAME Fortran_reserved COMMAND test_reserved_fortran)
endif()

get_property(test_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY TESTS)

set_property(TEST ${test_names} PROPERTY DISABLED $<NOT:$<BOOL:${HAVE_CXX_FILESYSTEM}>>)
