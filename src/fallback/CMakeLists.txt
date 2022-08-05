add_library(filesystem_c OBJECT filesystem.c)
target_include_directories(filesystem_c PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_definitions(filesystem_c PRIVATE $<$<BOOL:${MSVC}>:_CRT_SECURE_NO_WARNINGS>)

set(s ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(filesystem PRIVATE
${s}/filesystem.f90 ${s}/general.f90 ${s}/crt.f90 ${s}/fs_c.f90
$<TARGET_OBJECTS:filesystem_c>
)
