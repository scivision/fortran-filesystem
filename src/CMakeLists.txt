set_directory_properties(PROPERTIES LABELS pathlib)

add_library(pathlib pathlib.f90 envvar.f90)
if(WIN32)
  target_sources(pathlib PRIVATE windows_crt.f90 windows_path.f90)
else()
  target_sources(pathlib PRIVATE posix_crt.f90 posix_path.f90)
endif()
if(CMAKE_Fortran_COMPILER_ID STREQUAL GNU)
  target_sources(pathlib PRIVATE gcc.f90)
elseif(CMAKE_Fortran_COMPILER_ID MATCHES "^Intel")
  target_sources(pathlib PRIVATE intel.f90)
else()
  target_sources(pathlib PRIVATE dummy_compiler.f90)
endif()
target_include_directories(pathlib INTERFACE ${PROJECT_BINARY_DIR}/include)

# realpath canonicalization via C Runtime Library
target_sources(pathlib PRIVATE
$<IF:$<BOOL:${WIN32}>,canonical_windows.f90,canonical_posix.f90>
)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
