set_directory_properties(PROPERTIES LABELS pathlib)

target_sources(pathlib PRIVATE pathlib.f90 iter.f90 io.f90 find.f90)

# --- C++17 filesystem OR lstat() symbolic link information

if(HAVE_CXX17_FILESYSTEM)
  target_sources(pathlib PRIVATE fs_cpp.f90 fs.cpp)
  target_compile_definitions(pathlib PRIVATE $<$<BOOL:${MSVC}>:_CRT_SECURE_NO_WARNINGS>)
  target_link_libraries(pathlib PUBLIC ${libfs})
else()
  add_subdirectory(fallback)
endif()
