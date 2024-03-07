if(NOT HAVE_CXX_FILESYSTEM)
  return()
endif()

if(TARGET fs_cli)

add_test(NAME Cpp_CLI_Cpp
COMMAND ${CMAKE_COMMAND}
-Dexe=$<TARGET_FILE:fs_cli>
"-Dregex=Ffilesystem\> 1"
-P ${CMAKE_CURRENT_SOURCE_DIR}/../stdin.cmake
)

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
