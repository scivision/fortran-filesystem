add_compile_definitions($<$<BOOL:${WIN32}>:_CRT_SECURE_NO_WARNINGS>)

set(${PROJECT_NAME}_fortran_test_flags
  "$<$<AND:$<Fortran_COMPILER_ID:GNU>,$<VERSION_LESS:$<Fortran_COMPILER_VERSION>,13>>:-Wno-maybe-uninitialized>"
)

if(ffilesystem_bench)
  add_library(ffilesystem_bench OBJECT ffilesystem_bench.cpp)
  target_include_directories(ffilesystem_bench PRIVATE ${PROJECT_SOURCE_DIR}/include
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
  target_compile_definitions(ffilesystem_bench PRIVATE HAVE_CXX_FILESYSTEM=$<BOOL:${HAVE_CXX_FILESYSTEM}>)
endif()

foreach(t IN ITEMS canonical_resolve cpp fortran core empty environment
  executable exepath
  expanduser fileops libpath mkdtemp overflow permissions reserved symlink windows)

add_subdirectory(${t})

# tests run much faster in serial, especially on Windows
get_property(n DIRECTORY ${t} PROPERTY TESTS)

# --- Windows shared DLLs
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.28)

  set_property(TEST ${n} DIRECTORY ${t} PROPERTY RUN_SERIAL true)

  if(BUILD_SHARED_LIBS AND (CYGWIN OR WIN32))
    set_property(TEST ${n} DIRECTORY ${t}
    PROPERTY ENVIRONMENT_MODIFICATION "PATH=path_list_prepend:$<TARGET_FILE_DIR:ffilesystem>"
    )
  endif()
endif()

endforeach()

# --- coverage

if(${PROJECT_NAME}_coverage)
  setup_target_for_coverage_gcovr_html(
  NAME coverage
  EXECUTABLE ${CMAKE_CTEST_COMMAND}
  )
endif()
