# Check compiler's C++17 capabilities

function(cpp_check)

# is the compiler basically C++17 enabled?
check_cxx_symbol_exists(__has_include "" HAVE_INCLUDE_MACRO)
if(NOT HAVE_INCLUDE_MACRO)
  message(WARNING "C++ compiler not C++17 capable ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
  return()
endif()

# https://en.cppreference.com/w/cpp/feature_test
check_cxx_symbol_exists(__cpp_lib_filesystem filesystem HAVE_FS_FEATURE)

if(NOT HAVE_FS_FEATURE)
  message(WARNING "C++ filesystem feature is not available with ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
  return()
endif()

if(HAVE_FS_FEATURE)
  # some compilers e.g. Cray claim to have filesystem, but their libstdc++ doesn't have it.
  check_cxx_source_compiles([=[
  #if __has_include(<filesystem>)
  #include <filesystem>
  namespace fs = std::filesystem;
  #else
  #error "No C++ filesystem support"
  #endif

  int main () {
  fs::path tgt(".");
  auto h = tgt.has_filename();
  return EXIT_SUCCESS;
  }
  ]=]
  HAVE_CXX_FILESYSTEM
  )

  if(NOT HAVE_CXX_FILESYSTEM)
    message(WARNING "C++ compiler has filesystem feature, but filesystem is broken in libstdc++ ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
    return()
  endif()

endif()

endfunction()
