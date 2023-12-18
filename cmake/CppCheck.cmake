# Check compiler's C++17 capabilities

function(cpp_check)

# https://en.cppreference.com/w/cpp/feature_test
check_cxx_symbol_exists(__cpp_lib_filesystem "filesystem" HAVE_FS_FEATURE)

if(NOT HAVE_FS_FEATURE)
  message(WARNING "C++ filesystem feature is not available with ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
  return()
endif()

# some compilers e.g. Cray claim to have filesystem, but their libstdc++ doesn't have it.
check_cxx_source_compiles([=[
#include <cstdlib>
#include <filesystem>

static_assert(__cpp_lib_filesystem, "No C++ filesystem support");

namespace fs = std::filesystem;


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

check_cxx_symbol_exists(__cpp_lib_make_unique "memory" cpp14_make_unique)
if(NOT cpp14_make_unique)
  message(WARNING "C++ compiler has filesystem feature, but lacks C++14 std::make_unique()")
  set(HAVE_CXX_FILESYSTEM false CACHE BOOL "C++14 make_unique is missing" FORCE)
  return()
endif()

# e.g. AppleClang 15 doesn't yet have this, maybe not worth the bother
# i.e. benchmarking may reveal miniscule benefit.
# check_cxx_symbol_exists(__cpp_lib_smart_ptr_for_overwrite "memory" cpp20_smart_ptr_for_overwrite)

# informational for dev users
if(CMAKE_CXX_STANDARD GREATER_EQUAL 20)
  check_cxx_symbol_exists(__cpp_lib_format "format" cpp20_format)
endif()

if(NOT cpp20_format)
  message(STATUS "fs_compiler() will return empty as compiler doesn't have C++20 std::format")
endif()

if(CMAKE_CXX_STANDARD GREATER_EQUAL 20)
  check_cxx_symbol_exists(__cpp_lib_starts_ends_with "string" cpp20_string_ends_with)
endif()

endfunction()
