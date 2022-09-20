set_property(DIRECTORY PROPERTY LABELS cpp)

add_executable(test_cpp_ifc test_ifc.cpp)
target_compile_features(test_cpp_ifc PRIVATE cxx_std_17)
target_link_libraries(test_cpp_ifc PRIVATE filesystem)
add_test(NAME CPP_interface COMMAND test_cpp_ifc)


# --- props
get_property(test_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY TESTS)
set_property(TEST ${test_names} PROPERTY RUN_SERIAL true)
set_property(TEST ${test_names} PROPERTY TIMEOUT 15)
# tests run much faster in serial, especially on Windows

get_property(test_targets DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)
foreach(t IN LISTS test_targets)
  # Intel needs linker_language fortran else error "undefined reference to `main'"
  set_property(TARGET ${t} PROPERTY LINKER_LANGUAGE Fortran)
endforeach()
