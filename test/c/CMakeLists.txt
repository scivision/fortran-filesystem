# --- C
foreach(t IN ITEMS canonical expanduser file env)
  add_executable(test_C_${t} test_${t}.c)
  target_link_libraries(test_C_${t} PRIVATE ffilesystem)
  add_test(NAME C_${t} COMMAND test_C_${t})
  set_property(TEST C_${t} PROPERTY LABELS "C")
endforeach()

if(NOT BSD)
add_executable(test_exepath_c test_exepath.c)
target_link_libraries(test_exepath_c PRIVATE ffilesystem)
add_test(NAME C_exepath
COMMAND test_exepath_c $<TARGET_FILE_BASE_NAME:test_exepath_c>
)
endif()

add_executable(test_libpath_c test_libpath.c)
target_link_libraries(test_libpath_c PRIVATE ffilesystem)
add_test(NAME C_libpath
COMMAND test_libpath_c
  $<AND:$<BOOL:${BUILD_SHARED_LIBS}>,$<OR:$<BOOL:${WIN32}>,$<BOOL:${CYGWIN}>,$<BOOL:${HAVE_DLADDR}>>>
  $<TARGET_FILE_NAME:ffilesystem>
)

# --- C++
if(HAVE_CXX_FILESYSTEM)

if(ffilesystem_cli)
add_test(NAME Cpp_CLI_Cpp COMMAND fs_cli cpp)
set_property(TEST Cpp_CLI_Cpp PROPERTY PASS_REGULAR_EXPRESSION "^1")
endif()

foreach(t IN ITEMS canonical core exe utf8)
  add_executable(test_Cpp_${t} test_${t}.cpp)
  target_link_libraries(test_Cpp_${t} PRIVATE ffilesystem)
  add_test(NAME Cpp_${t} COMMAND test_Cpp_${t})
  set_property(TEST Cpp_${t} PROPERTY LABELS "Cpp")
endforeach()

add_executable(cpp_env test_env.cpp)
target_link_libraries(cpp_env PRIVATE ffilesystem)
add_test(NAME CppHomedirEnv COMMAND cpp_env)


if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.22)
  add_test(NAME CppHomedirNoEnv COMMAND cpp_env)
  set_property(TEST CppHomedirNoEnv PROPERTY ENVIRONMENT_MODIFICATION "HOME=unset:;USERPROFILE=unset:;TEMP=unset:;TMPDIR=unset:")
  set_property(TEST CppHomedirNoEnv PROPERTY DISABLED $<BOOL:$ENV{CI}>)
endif()

# --- binpath

if(NOT BSD)
add_executable(test_exepath_cpp test_exepath.cpp)
target_link_libraries(test_exepath_cpp PRIVATE ffilesystem)
add_test(NAME Cpp_exepath
COMMAND test_exepath_cpp $<TARGET_FILE_BASE_NAME:test_exepath_cpp>
)
endif()

add_executable(test_libpath_cpp test_libpath.cpp)
target_link_libraries(test_libpath_cpp PRIVATE ffilesystem)
add_test(NAME Cpp_libpath
COMMAND test_libpath_cpp
  $<AND:$<BOOL:${BUILD_SHARED_LIBS}>,$<OR:$<BOOL:${WIN32}>,$<BOOL:${CYGWIN}>,$<BOOL:${HAVE_DLADDR}>>>
  $<TARGET_FILE_NAME:ffilesystem>
)


endif()

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

get_property(test_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY TESTS)
set_property(TEST ${test_names} PROPERTY RUN_SERIAL true)
# tests run much faster in serial, especially on Windows

# --- Windows shared DLLs
if((CYGWIN OR WIN32) AND BUILD_SHARED_LIBS AND CMAKE_VERSION VERSION_GREATER_EQUAL 3.22)
  set_property(TEST ${test_names} PROPERTY ENVIRONMENT_MODIFICATION "PATH=path_list_append:$<TARGET_FILE_DIR:ffilesystem>")
endif()
