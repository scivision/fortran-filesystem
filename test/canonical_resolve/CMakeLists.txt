if(ffilesystem_bench)

add_executable(bench_canonical bench_canonical.cpp)
target_link_libraries(bench_canonical PRIVATE ffilesystem)
target_compile_definitions(bench_canonical PRIVATE HAVE_CXX_FILESYSTEM=$<BOOL:${HAVE_CXX_FILESYSTEM}>)

add_test(NAME bench_canonical COMMAND bench_canonical)
set_tests_properties(bench_canonical PROPERTIES
LABELS "bench"
RUN_SERIAL TRUE
)

endif()


if(HAVE_CXX_FILESYSTEM)

add_executable(test_canonical_cpp test_canonical.cpp)
target_link_libraries(test_canonical_cpp PRIVATE ffilesystem)

add_test(NAME test_canonical_cpp COMMAND test_canonical_cpp)
set_property(TEST test_canonical_cpp PROPERTY PROPERTY LABELS "Cpp")

endif()


if(HAVE_Fortran_FILESYSTEM)

foreach(t IN ITEMS canonical resolve)
  add_executable(Fortran_${t} test_${t}.f90)
  target_link_libraries(Fortran_${t} PRIVATE ffilesystem)
  set_property(TARGET Fortran_${t} PROPERTY LINKER_LANGUAGE Fortran)

  add_test(NAME Fortran_${t} COMMAND Fortran_${t})
  set_property(TEST Fortran_${t} PROPERTY PROPERTY LABELS "Fortran")
endforeach()


endif()
