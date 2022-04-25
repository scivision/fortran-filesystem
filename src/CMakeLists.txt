# --- filesystem binary
if(HAVE_SYMLINK)
  set(has_symlink .true.)
else()
  set(has_symlink .false.)
endif()
if(HAVE_CXX_FILESYSTEM)
  set(cpp_full_filesystem .true.)
else()
  set(cpp_full_filesystem .false.)
endif()
configure_file(platform.in.f90 platform.f90 @ONLY)

add_library(filesystem
filesystem.f90 iter.f90 io.f90 find.f90
${CMAKE_CURRENT_BINARY_DIR}/platform.f90
)
set_target_properties(filesystem PROPERTIES
Fortran_MODULE_DIRECTORY ${PROJECT_BINARY_DIR}/include
RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
)
target_include_directories(filesystem PUBLIC
$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
$<INSTALL_INTERFACE:include>
)

# --- C++17 filesystem
add_library(filesystem_cpp OBJECT fs.cpp)
target_compile_features(filesystem_cpp PRIVATE cxx_std_17)
target_compile_definitions(filesystem_cpp PRIVATE $<$<BOOL:${MSVC}>:_CRT_SECURE_NO_WARNINGS>)

target_sources(filesystem PRIVATE fs_cpp.f90 $<TARGET_OBJECTS:filesystem_cpp>)
target_link_libraries(filesystem INTERFACE ${CMAKE_REQUIRED_LIBRARIES})
