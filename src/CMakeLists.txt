set_directory_properties(PROPERTIES LABELS pathlib)

add_library(pathlib
pathlib.f90
$<IF:$<BOOL:${WIN32}>,pathlib_windows.f90,pathlib_unix.f90>
$<$<Fortran_COMPILER_ID:Intel,IntelLLVM>:pathlib_intel.f90>
$<$<Fortran_COMPILER_ID:GNU>:pathlib_gcc.f90>
$<$<NOT:$<Fortran_COMPILER_ID:GNU,Intel,IntelLLVM>>:pathlib_dummy.f90>
)
target_include_directories(pathlib INTERFACE ${PROJECT_BINARY_DIR}/include)

# realpath canonicalization via C Runtime Library
target_sources(pathlib PRIVATE
$<IF:$<BOOL:${WIN32}>,canonical_windows.f90,canonical_posix.f90>
)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
