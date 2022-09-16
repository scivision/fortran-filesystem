add_library(filesystem_c OBJECT filesystem.c)
target_include_directories(filesystem_c PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_definitions(filesystem_c PRIVATE
$<$<BOOL:${MSVC}>:_CRT_SECURE_NO_WARNINGS>
)
target_compile_features(filesystem_c PRIVATE c_std_99)

#--- optional lib_path()
if(WIN32)
  target_compile_definitions(filesystem_c PRIVATE FS_DLL_NAME="$<TARGET_FILE_NAME:filesystem>")
elseif(HAVE_DLADDR)
  target_link_libraries(filesystem_c PRIVATE ${CMAKE_DL_LIBS})
  target_compile_definitions(filesystem_c PRIVATE _GNU_SOURCE HAVE_DLADDR)
endif()

#--- sources
set(s ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(filesystem PRIVATE
${s}/filesystem.f90 ${s}/general.f90 ${s}/crt.f90 ${s}/fs_c.f90
$<TARGET_OBJECTS:filesystem_c>
)
