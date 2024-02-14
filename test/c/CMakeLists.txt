# --- C
foreach(t IN ITEMS canonical expanduser file env)
  add_executable(test_C_${t} test_${t}.c)
  target_link_libraries(test_C_${t} PRIVATE ffilesystem)
  add_test(NAME C_${t} COMMAND test_C_${t})
  set_property(TEST C_${t} PROPERTY LABELS "C")
endforeach()

# --- C++
if(HAVE_CXX_FILESYSTEM)

if(ffilesystem_cli)
add_test(NAME Cpp_CLI_Cpp COMMAND fs_cli cpp)
set_property(TEST Cpp_CLI_Cpp PROPERTY PASS_REGULAR_EXPRESSION "^1")
endif()

add_executable(test_Cpp_windows test_windows.cpp)
target_link_libraries(test_Cpp_windows PRIVATE ffilesystem)

add_test(NAME Cpp_windows COMMAND test_Cpp_windows)
set_property(TEST Cpp_windows PROPERTY DISABLED $<NOT:$<BOOL:${WIN32}>>)

foreach(t IN ITEMS canonical core utf8)
  add_executable(test_Cpp_${t} test_${t}.cpp)
  target_link_libraries(test_Cpp_${t} PRIVATE ffilesystem)
  add_test(NAME Cpp_${t} COMMAND test_Cpp_${t})
  set_property(TEST Cpp_${t} PROPERTY LABELS "Cpp")
endforeach()

# --- homedir
add_executable(cpp_env test_env.cpp)
target_link_libraries(cpp_env PRIVATE ffilesystem)
add_test(NAME CppHomedirEnv COMMAND cpp_env)


if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.22)
  add_test(NAME CppHomedirNoEnv COMMAND cpp_env)
  set_property(TEST CppHomedirNoEnv PROPERTY ENVIRONMENT_MODIFICATION "HOME=unset:;USERPROFILE=unset:;TEMP=unset:;TMPDIR=unset:")
  set_property(TEST CppHomedirNoEnv PROPERTY DISABLED $<BOOL:$ENV{CI}>)
endif()

# --- permissions

set(perm_noread ${CMAKE_CURRENT_BINARY_DIR}/ffilesystem_permissions_noread)
set(perm_nowrite ${CMAKE_CURRENT_BINARY_DIR}/ffilesystem_permissions_nowrite)

add_test(NAME Cpp_noread_setup
COMMAND ${CMAKE_COMMAND}
  -Dperm_noread:FILEPATH=${perm_noread}
  -Dperm_nowrite:FILEPATH=${perm_nowrite}
  -P ${CMAKE_CURRENT_SOURCE_DIR}/../test_permissions_setup.cmake)
set_property(TEST Cpp_noread_setup PROPERTY FIXTURES_SETUP Cpp_noread_fxt)
set_property(TEST Cpp_noread_setup PROPERTY SKIP_RETURN_CODE 77)

add_executable(test_permissions_cpp test_permissions.cpp)
target_link_libraries(test_permissions_cpp PRIVATE ffilesystem)
add_test(NAME Cpp_permissions
COMMAND test_permissions_cpp $<TARGET_FILE:test_permissions_cpp> ${perm_noread} ${perm_nowrite}
)
set_property(TEST Cpp_permissions PROPERTY SKIP_RETURN_CODE 77)
set_property(TEST Cpp_permissions PROPERTY DISABLED $<VERSION_LESS:${CMAKE_VERSION},3.19>)
set_property(TEST Cpp_permissions PROPERTY FIXTURES_REQUIRED Cpp_noread_fxt)

add_test(NAME Cpp_noread_cleanup COMMAND ${CMAKE_COMMAND} -E rm ${perm_noread} ${perm_nowrite})
set_property(TEST Cpp_noread_cleanup PROPERTY DISABLED $<VERSION_LESS:${CMAKE_VERSION},3.19>)
set_property(TEST Cpp_noread_cleanup PROPERTY FIXTURES_CLEANUP Cpp_noread_fxt)

# --- executable

set(perm_noexe ${CMAKE_CURRENT_BINARY_DIR}/ffilesystem_permissions_noexe)

add_test(NAME Cpp_noexe_setup
COMMAND ${CMAKE_COMMAND} -Dperm_noexe:FILEPATH=${perm_noexe}
  -P ${CMAKE_CURRENT_SOURCE_DIR}/../test_permissions_setup.cmake)
set_property(TEST Cpp_noexe_setup PROPERTY FIXTURES_SETUP Cpp_noexe_fxt)
set_property(TEST Cpp_noexe_setup PROPERTY SKIP_RETURN_CODE 77)

add_executable(test_exe_cpp test_exe.cpp)
target_link_libraries(test_exe_cpp PRIVATE ffilesystem)
add_test(NAME Cpp_exe
COMMAND test_exe_cpp $<TARGET_FILE:test_exe_cpp> ${perm_noexe}
)
set_property(TEST Cpp_exe PROPERTY SKIP_RETURN_CODE 77)
set_property(TEST Cpp_exe PROPERTY DISABLED $<VERSION_LESS:${CMAKE_VERSION},3.19>)
set_property(TEST Cpp_exe PROPERTY FIXTURES_REQUIRED Cpp_noexe_fxt)

add_test(NAME Cpp_noexe_cleanup COMMAND ${CMAKE_COMMAND} -E rm ${perm_noexe})
set_property(TEST Cpp_noexe_cleanup PROPERTY DISABLED $<VERSION_LESS:${CMAKE_VERSION},3.19>)
set_property(TEST Cpp_noexe_cleanup PROPERTY FIXTURES_CLEANUP Cpp_noexe_fxt)


# --- binpath

add_executable(test_exepath_cpp test_exepath.cpp)
target_link_libraries(test_exepath_cpp PRIVATE ffilesystem)
add_test(NAME Cpp_exepath
COMMAND test_exepath_cpp $<TARGET_FILE_BASE_NAME:test_exepath_cpp>
)

add_executable(test_libpath_cpp test_libpath.cpp)
target_link_libraries(test_libpath_cpp PRIVATE ffilesystem)
add_test(NAME Cpp_libpath
COMMAND test_libpath_cpp
  $<AND:$<BOOL:${BUILD_SHARED_LIBS}>,$<OR:$<BOOL:${WIN32}>,$<BOOL:${CYGWIN}>,$<BOOL:${HAVE_DLADDR}>>>
  $<TARGET_FILE_NAME:ffilesystem>
)
set_property(TEST Cpp_libpath Cpp_exepath PROPERTY SKIP_RETURN_CODE 77)
set_property(TEST Cpp_libpath Cpp_exepath PROPERTY SKIP_REGULAR_EXPRESSION "not implemented;not available")


endif(HAVE_CXX_FILESYSTEM)

# --- shaky tests
foreach(t IN ITEMS empty reserved)
  add_executable(test_C_${t} test_${t}.c)
  target_link_libraries(test_C_${t} PRIVATE ffilesystem)
  add_test(NAME C_${t} COMMAND ${CMAKE_COMMAND} -E env $<TARGET_FILE:test_C_${t}>)
  if(ffilesystem_shaky)
    set_property(TEST C_${t} PROPERTY SKIP_REGULAR_EXPRESSION "(uncaught exception|[bB]us error|SIGABRT)")
    set_property(TEST C_${t} PROPERTY LABELS "shaky")
  endif()
endforeach()



# --- props

set_property(TEST C_reserved C_file
PROPERTY SKIP_REGULAR_EXPRESSION "not implemented")

if(NOT HAVE_CXX_FILESYSTEM)
  set_property(TEST C_reserved PROPERTY DISABLED $<$<BOOL:${WIN32}>:true>)
endif()


get_property(test_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY TESTS)
set_property(TEST ${test_names} PROPERTY RUN_SERIAL true)
# tests run much faster in serial, especially on Windows

# --- Windows shared DLLs
if((CYGWIN OR WIN32) AND BUILD_SHARED_LIBS AND CMAKE_VERSION VERSION_GREATER_EQUAL 3.22)
  set_property(TEST ${test_names} PROPERTY ENVIRONMENT_MODIFICATION "PATH=path_list_append:$<TARGET_FILE_DIR:ffilesystem>")
endif()
