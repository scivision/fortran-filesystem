set_directory_properties(PROPERTIES LABELS pathlib)

add_library(pathlib
pathlib.f90
$<IF:$<BOOL:${WIN32}>,pathlib_windows.f90,pathlib_unix.f90>
$<$<Fortran_COMPILER_ID:Intel,IntelLLVM>:pathlib_intel.f90>
$<$<Fortran_COMPILER_ID:GNU>:pathlib_gcc.f90>
$<$<NOT:$<Fortran_COMPILER_ID:GNU,Intel,IntelLLVM>>:pathlib_dummy.f90>
)

if(BUILD_TESTING)
  add_executable(test_pathlib test_pathlib.f90)
  target_link_libraries(test_pathlib PRIVATE pathlib)
  target_compile_options(test_pathlib PRIVATE $<$<Fortran_COMPILER_ID:GNU>:-Wno-intrinsic-shadow>)

  add_test(NAME pathlib COMMAND test_pathlib)
  set_tests_properties(pathlib PROPERTIES TIMEOUT 10)
endif()
