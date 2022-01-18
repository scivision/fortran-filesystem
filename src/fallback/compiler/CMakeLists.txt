if(CMAKE_Fortran_COMPILER_ID STREQUAL GNU)
  target_sources(pathlib PRIVATE gcc.f90)
elseif(CMAKE_Fortran_COMPILER_ID MATCHES "^Intel")
  target_sources(pathlib PRIVATE intel.f90)
else()
  target_sources(pathlib PRIVATE unknown.f90)
endif()

target_sources(pathlib PRIVATE no_fs_cpp.f90)
