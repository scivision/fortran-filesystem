add_library(fs_c OBJECT filesystem.c)
target_include_directories(fs_c PRIVATE ${PROJECT_SOURCE_DIR}/include)

add_library(filesystem filesystem.F90 general.f90 $<TARGET_OBJECTS:fs_c>)
target_compile_definitions(filesystem PRIVATE $<$<BOOL:${WIN32}>:_WIN32>)
# Gfortran preprocessor fails to define C/C++ expected macros

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
