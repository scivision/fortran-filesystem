set_directory_properties(PROPERTIES LABELS pathlib)

target_sources(pathlib PRIVATE pathlib.f90 pure.f90 pure_iter.f90 impure.f90 envvar.f90)
if(WIN32)
  add_subdirectory(windows)
else()
  add_subdirectory(posix)
endif()

add_subdirectory(compiler)
