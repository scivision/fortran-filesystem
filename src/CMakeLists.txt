set_directory_properties(PROPERTIES LABELS pathlib)

target_sources(pathlib PRIVATE pathlib.f90 pure.f90 pure_iter.f90 impure.f90
io.f90 find.f90 envvar.f90)

# --- C++17 filesystem OR lstat() symbolic link information

if(HAVE_CXX17_FILESYSTEM)
  target_sources(pathlib PRIVATE fs.cpp fs_cpp.f90)
  target_link_libraries(pathlib PUBLIC ${libfs})
else()
  if(CMAKE_Fortran_COMPILER_ID STREQUAL GNU)
    target_sources(pathlib PRIVATE compiler/gcc_no_cpp_fs.f90)
  elseif(CMAKE_Fortran_COMPILER_ID MATCHES "^Intel")
    target_sources(pathlib PRIVATE compiler/intel_no_cpp_fs.f90)
  endif()

  if(WIN32)
    target_sources(pathlib PRIVATE windows/crt.f90 windows/sys.f90)
  else()
    target_sources(pathlib PRIVATE posix/crt.f90 posix/sys.f90)
  endif()

  target_sources(pathlib PRIVATE compiler/no_fs_cpp.f90)
endif()

# --- OS or OS-specific compiler dependent

if(WIN32)
  add_subdirectory(windows)
else()
  add_subdirectory(posix)
endif()

# --- OS-agnostic compiler depedent

add_subdirectory(compiler)
