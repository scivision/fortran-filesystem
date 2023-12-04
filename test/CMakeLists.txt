add_subdirectory(c)

if(ffilesystem_fortran)
  add_subdirectory(fortran)
endif()


# --- coverage

if(coverage)
  setup_target_for_coverage_gcovr_html(
  NAME coverage
  EXECUTABLE ${CMAKE_CTEST_COMMAND}
  )
endif()
