set_property(DIRECTORY PROPERTY LABELS fortran)

if(CMAKE_Fortran_COMPILER_ID STREQUAL "GNU")
  add_compile_options($<$<COMPILE_LANGUAGE:Fortran>:-Wno-maybe-uninitialized>)
endif()

foreach(t IN ITEMS canonical core env exe expanduser
        fileop filesize find relative reserved same symlink)
  add_executable(test_${t} test_${t}.f90)
  target_link_libraries(test_${t} PRIVATE ffilesystem)
  add_test(NAME Fortran_${t} COMMAND test_${t})
endforeach()

set_property(TEST Fortran_core PROPERTY FIXTURES_SETUP core_fxt)

if(HAVE_CXX_FILESYSTEM)
  set_property(TEST Fortran_relative PROPERTY PASS_REGULAR_EXPRESSION "OK: relative_to full")
  set_property(TEST Fortran_canonical PROPERTY PASS_REGULAR_EXPRESSION "OK: canonical full")
endif()

set_property(TEST Fortran_fileop PROPERTY WORKING_DIRECTORY $<TARGET_FILE_DIR:test_fileop>)
set_property(TEST Fortran_fileop PROPERTY FIXTURES_SETUP ops_fxt)
# workdir avoids test state being shared between tests

set_property(TEST Fortran_find PROPERTY FIXTURES_REQUIRED ops_fxt)

# --- binpath
add_executable(test_binpath_fortran test_binpath.f90)
target_link_libraries(test_binpath_fortran PRIVATE ffilesystem)
add_test(NAME Fortran_binpath
COMMAND test_binpath_fortran $<BOOL:${BUILD_SHARED_LIBS}> $<TARGET_FILE_NAME:test_binpath_fortran>  $<TARGET_FILE_NAME:ffilesystem>
)


# --- props
set_property(TEST Fortran_relative Fortran_canonical Fortran_filesize PROPERTY FIXTURES_REQUIRED core_fxt)

get_property(test_targets DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)
# Intel needs linker_language fortran else error "undefined reference to `main'"
set_property(TARGET ${test_targets} PROPERTY LINKER_LANGUAGE Fortran)

get_property(test_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY TESTS)
set_property(TEST ${test_names} PROPERTY RUN_SERIAL true)
# tests run much faster in serial, especially on Windows

# --- Windows shared DLLs
if(WIN32 AND CMAKE_VERSION VERSION_GREATER_EQUAL 3.22)
  dll_test_path(filesystem "${test_names}")
  set_property(TEST ${test_names} PROPERTY ENVIRONMENT_MODIFICATION "PATH=path_list_append:$<TARGET_FILE_DIR:ffilesystem>")
endif()
