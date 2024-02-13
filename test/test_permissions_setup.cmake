if(CMAKE_VERSION VERSION_LESS 3.19)
  return()
endif()

if(NOT DEFINED perm_noread)
  message(FATAL_ERROR "must specify variable perm_noread")
endif()

file(REMOVE ${perm_noread})
    # need this logic because perm_noread is non-readable and
    # kwSys:SystemTools:FileExists is false for non-readable files
file(TOUCH ${perm_noread})
file(CHMOD ${perm_noread} FILE_PERMISSIONS OWNER_EXECUTE)

if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.29)
  if(IS_READABLE ${perm_noread})
    message(WARNING "${perm_noread} should not be readable")
    cmake_language(EXIT 77)
  endif()
endif()
