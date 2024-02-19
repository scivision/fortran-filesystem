if(HAVE_CXX_FILESYSTEM)

add_executable(test_Cpp_windows test_windows.cpp)
target_link_libraries(test_Cpp_windows PRIVATE ffilesystem)

add_test(NAME Cpp_windows COMMAND test_Cpp_windows)

endif()


if(ffilesystem_fortran)

add_executable(test_Fortran_windows test_windows.f90)
target_link_libraries(test_Fortran_windows PRIVATE ffilesystem)
set_property(TARGET test_Fortran_windows PROPERTY LINKER_LANGUAGE Fortran)

add_test(NAME Fortran_Ffilesystem_windows COMMAND test_Fortran_windows)
endif()


get_property(test_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY TESTS)

set_property(TEST ${test_names} PROPERTY DISABLED $<OR:$<NOT:$<BOOL:${WIN32}>>,$<NOT:$<BOOL:${HAVE_CXX_FILESYSTEM}>>>)
