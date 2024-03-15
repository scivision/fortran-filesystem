if(HAVE_CXX_FILESYSTEM)

add_executable(test_expanduser_cpp test_expanduser.cpp)
target_link_libraries(test_expanduser_cpp PRIVATE ffilesystem)

add_test(NAME test_expanduser_cpp COMMAND test_expanduser_cpp)
set_property(TEST test_expanduser_cpp PROPERTY LABELS "Cpp")

endif()


if(HAVE_Fortran_FILESYSTEM)

add_executable(test_expanduser_fortran test_expanduser.f90)
target_link_libraries(test_expanduser_fortran PRIVATE ffilesystem)

add_test(NAME test_expanduser_fortran COMMAND test_expanduser_fortran)
set_property(TEST test_expanduser_fortran PROPERTY LABELS "Fortran")

endif()
