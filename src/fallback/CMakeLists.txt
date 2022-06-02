add_library(filesystem_c OBJECT filesystem.c)
target_include_directories(filesystem_c PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_definitions(filesystem_c PRIVATE $<$<BOOL:${MSVC}>:_CRT_SECURE_NO_WARNINGS>)

add_library(filesystem filesystem.F90 general.f90 crt.f90
fs_c.f90 $<TARGET_OBJECTS:filesystem_c>
)
target_compile_definitions(filesystem PRIVATE $<$<BOOL:${WIN32}>:_WIN32>)
# Gfortran preprocessor fails to define C/C++ expected macros
target_compile_options(filesystem PRIVATE
"$<$<Fortran_COMPILER_ID:GNU>:-Wno-unused-dummy-argument;-Wno-return-type>"
"$<$<Fortran_COMPILER_ID:Intel,IntelLLVM>:$<IF:$<BOOL:${WIN32}>,/Qdiag-disable:6178$<COMMA>7712,-diag-disable=6178$<COMMA>7712>>"
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
