target_sources(pathlib PRIVATE general.f90)

if(WIN32)
  add_subdirectory(windows)
else()
  add_subdirectory(posix)
endif()

# --- OS-agnostic compiler depedent

add_subdirectory(compiler)
