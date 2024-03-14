if(HAVE_CXX_FILESYSTEM)
add_executable(test_libpath_cpp test_libpath.cpp)
target_link_libraries(test_libpath_cpp PRIVATE ffilesystem)
add_test(NAME Cpp_libpath
COMMAND test_libpath_cpp
  $<AND:$<BOOL:${BUILD_SHARED_LIBS}>,$<OR:$<BOOL:${WIN32}>,$<BOOL:${CYGWIN}>,$<BOOL:${HAVE_DLADDR}>>>
  $<TARGET_FILE_NAME:ffilesystem>
)
endif()

if(HAVE_Fortran_FILESYSTEM)
add_executable(test_libpath_fortran test_libpath.f90)
target_link_libraries(test_libpath_fortran PRIVATE ffilesystem)
target_compile_options(test_libpath_fortran PRIVATE ${${PROJECT_NAME}_fortran_test_flags})
set_property(TARGET test_libpath_fortran PROPERTY LINKER_LANGUAGE Fortran)

add_test(NAME Fortran_libpath
COMMAND test_libpath_fortran
  $<AND:$<BOOL:${BUILD_SHARED_LIBS}>,$<OR:$<BOOL:${WIN32}>,$<BOOL:${CYGWIN}>,$<BOOL:${HAVE_DLADDR}>>>
  $<TARGET_FILE_NAME:ffilesystem>
)
endif()



get_property(test_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY TESTS)

set_tests_properties(${test_names} PROPERTIES
SKIP_RETURN_CODE 77
SKIP_REGULAR_EXPRESSION "not implemented;not available"
DISABLED $<OR:$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>,$<NOT:$<BOOL:${HAVE_CXX_FILESYSTEM}>>>
)
