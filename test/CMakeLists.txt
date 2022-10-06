include(${PROJECT_SOURCE_DIR}/cmake/DllTestPath.cmake)

# --- C  interface
add_subdirectory(c)

# --- C++ interface
if(HAVE_CXX_FILESYSTEM)
  add_subdirectory(cpp)
endif()

if(fortran)
  add_subdirectory(fortran)
endif()


# --- coverage

if(coverage)
  setup_target_for_coverage_gcovr_html(
  NAME coverage
  EXECUTABLE ${CMAKE_CTEST_COMMAND}
  )
endif()
