if(ffilesystem_bench)

add_executable(bench_expanduser bench_expanduser.cpp)
target_link_libraries(bench_expanduser PRIVATE ffilesystem_bench ffilesystem)

add_test(NAME bench_expanduser COMMAND bench_expanduser)
set_tests_properties(bench_expanduser PROPERTIES
LABELS "bench"
RUN_SERIAL TRUE
)
endif()


if(HAVE_CXX_FILESYSTEM)

add_executable(test_expanduser_cpp test_expanduser.cpp)
target_link_libraries(test_expanduser_cpp PRIVATE ffilesystem)

add_test(NAME test_expanduser_cpp COMMAND test_expanduser_cpp)
set_property(TEST test_expanduser_cpp PROPERTY LABELS "Cpp")

endif()


if(HAVE_Fortran_FILESYSTEM)

add_executable(test_expanduser_fortran test_expanduser.f90)
target_link_libraries(test_expanduser_fortran PRIVATE ffilesystem)
target_compile_options(test_expanduser_fortran PRIVATE ${${PROJECT_NAME}_fortran_test_flags})
set_property(TARGET test_expanduser_fortran PROPERTY LINKER_LANGUAGE Fortran)

add_test(NAME test_expanduser_fortran COMMAND test_expanduser_fortran)
set_property(TEST test_expanduser_fortran PROPERTY LABELS "Fortran")

endif()
