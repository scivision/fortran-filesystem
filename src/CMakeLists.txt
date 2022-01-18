set_directory_properties(PROPERTIES LABELS pathlib)

target_sources(pathlib PRIVATE pathlib.f90 pure.f90 pure_iter.f90 impure.f90
io.f90 find.f90 envvar.f90)

# --- C++17 filesystem OR lstat() symbolic link information

if(HAVE_CXX17_FILESYSTEM)
  target_sources(pathlib PRIVATE fs.cpp fs_cpp.f90)
  target_link_libraries(pathlib PUBLIC ${libfs})
else()
  add_subdirectory(fallback)
endif()
