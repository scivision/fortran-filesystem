set(_noenv)

if(${PROJECT_NAME}_fortran)

add_executable(test_env test_env.f90)
target_link_libraries(test_env PRIVATE ffilesystem)
target_compile_options(test_env PRIVATE ${${PROJECT_NAME}_fortran_test_flags})
set_property(TARGET test_env PROPERTY LINKER_LANGUAGE Fortran)

add_test(NAME FortranHomedirEnv COMMAND test_env)
add_test(NAME FortranHomedirNoEnv COMMAND test_env)

list(APPEND _noenv FortranHomedirNoEnv)

endif()


if(HAVE_CXX_FILESYSTEM)

add_executable(cpp_env test_env.cpp)
target_link_libraries(cpp_env PRIVATE ffilesystem)

add_test(NAME CppHomedirEnv COMMAND cpp_env)
add_test(NAME CppHomedirNoEnv COMMAND cpp_env)

list(APPEND _noenv CppHomedirNoEnv)

endif()


if(CMAKE_VERSION VERSION_LESS 3.22 OR
  DEFINED ENV{CI} OR (WIN32 AND NOT HAVE_CXX_FILESYSTEM))
  set_property(TEST ${_noenv} PROPERTY DISABLED true)
else()
  set_property(TEST ${_noenv} PROPERTY ENVIRONMENT_MODIFICATION "HOME=unset:;USERPROFILE=unset:;TEMP=unset:;TMPDIR=unset:")
endif()