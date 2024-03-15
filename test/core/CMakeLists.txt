if(HAVE_Fortran_FILESYSTEM)

add_executable(test_fortran_core test_core.f90)
target_link_libraries(test_fortran_core PRIVATE ffilesystem)
target_compile_options(test_fortran_core PRIVATE ${${PROJECT_NAME}_fortran_test_flags})
set_property(TARGET test_fortran_core PROPERTY LINKER_LANGUAGE Fortran)

add_test(NAME Fortran_core COMMAND test_fortran_core)

endif()


if(HAVE_CXX_FILESYSTEM)

# --- C++
add_executable(test_cpp_core test_core.cpp)
target_link_libraries(test_cpp_core PRIVATE ffilesystem)

add_test(NAME Cpp_core COMMAND test_cpp_core)

endif()
