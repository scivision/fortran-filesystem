add_executable(test_reserved_c test_reserved.c)
target_link_libraries(test_reserved_c PRIVATE ffilesystem)

add_test(NAME C_reserved COMMAND ${CMAKE_COMMAND} -E env $<TARGET_FILE:test_reserved_c>)
# the -E env launcher is needed to catch subprocess abort for macOS with Clang (shaky exception handling in general)

set_property(TEST C_reserved PROPERTY SKIP_REGULAR_EXPRESSION "SIGABRT;[aA]bort")

if(ffilesystem_fortran)
add_executable(test_reserved_fortran test_reserved.f90)
target_link_libraries(test_reserved_fortran PRIVATE ffilesystem)
set_property(TARGET test_reserved_fortran PROPERTY LINKER_LANGUAGE Fortran)

add_test(NAME Fortran_reserved COMMAND test_reserved_fortran)
endif()

get_property(test_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY TESTS)

set_property(TEST ${test_names} PROPERTY DISABLED $<NOT:$<BOOL:${HAVE_CXX_FILESYSTEM}>>)
