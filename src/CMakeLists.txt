# --- filesystem binary

add_library(filesystem filesystem.f90 iter.f90 io.f90 find.f90)
set_target_properties(filesystem PROPERTIES
Fortran_MODULE_DIRECTORY ${PROJECT_BINARY_DIR}/include
RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
)
target_include_directories(filesystem PUBLIC
$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
$<INSTALL_INTERFACE:include>
)

# --- C++17 filesystem
add_library(filesystem_cpp OBJECT filesystem.cpp)
target_include_directories(filesystem_cpp PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_features(filesystem_cpp PRIVATE cxx_std_17)
target_compile_definitions(filesystem_cpp PRIVATE
$<$<BOOL:${MSVC}>:_CRT_SECURE_NO_WARNINGS>
$<$<BOOL:${HAVE_SYMLINK}>:HAVE_SYMLINK>
)

target_sources(filesystem PRIVATE fs_cpp.f90 $<TARGET_OBJECTS:filesystem_cpp>)
target_link_libraries(filesystem INTERFACE ${CMAKE_REQUIRED_LIBRARIES})
