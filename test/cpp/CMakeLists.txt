if(NOT HAVE_CXX_FILESYSTEM)
  return()
endif()

if(ffilesystem_cli)
add_test(NAME Cpp_CLI_Cpp COMMAND fs_cli cpp)
set_property(TEST Cpp_CLI_Cpp PROPERTY PASS_REGULAR_EXPRESSION "^1")
endif()

foreach(t IN ITEMS canonical expanduser file utf8)
  add_executable(test_cpp_${t} test_${t}.cpp)
  target_link_libraries(test_cpp_${t} PRIVATE ffilesystem)

  add_test(NAME Cpp_${t} COMMAND test_cpp_${t})
  set_property(TEST Cpp_${t} PROPERTY LABELS "Cpp")
endforeach()

# --- props

get_property(test_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY TESTS)
set_property(TEST ${test_names} PROPERTY RUN_SERIAL true)
# tests run much faster in serial, especially on Windows
