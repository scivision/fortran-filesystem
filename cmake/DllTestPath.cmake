function(locate_dll loc dll_mod)

foreach(l IN LISTS loc)
cmake_path(GET l PARENT_PATH ll)
if(IS_DIRECTORY ${ll})
  list(APPEND dll_mod "PATH=path_list_append:${ll}")
  set(${dll_mod} ${${dll_mod}} PARENT_SCOPE)

  cmake_path(SET d NORMALIZE ${ll}/../bin)
  # can't check bin/stem.dll as some libs add arbitrary stuff to stem
  if(IS_DIRECTORY ${d})
    list(APPEND dll_mod "PATH=path_list_append:${d}")
    set(${dll_mod} ${${dll_mod}} PARENT_SCOPE)
  endif()
endif()
endforeach()

endfunction(locate_dll)


function(dll_test_path libs test_names)
# if shared lib on Windows, need DLL on PATH

if(NOT WIN32 OR CMAKE_VERSION VERSION_LESS 3.22)
  return()
endif()


set(dll_mod)

foreach(lib IN LISTS libs)

  if(NOT TARGET ${lib})
    message(VERBOSE "${lib} is not a target, skipping")
    continue()
  endif()

  message(DEBUG "${lib} examining if needed")

  get_target_property(ttype ${lib} TYPE)
  if(ttype STREQUAL "STATIC_LIBRARY")
    message(DEBUG "${lib} is ${ttype}. No need for ENVIRONMENT_MODIFICATION for ${test_names}")
    continue()
  endif()

  foreach(t RELEASE RELWITHDEBINFO DEBUG NOCONFIG)
    get_target_property(imploc ${lib} IMPORTED_LOCATION_${t})
    if(imploc)
      break()
    endif()
  endforeach()

  get_target_property(intloc ${lib} INTERFACE_LINK_LIBRARIES)

  if(imploc)
    locate_dll(${imploc} dll_mod)
  elseif(intloc)
    locate_dll(${intloc} dll_mod)
  elseif(EXISTS ${lib})
    list(APPEND dll_mod "PATH=path_list_append:$<TARGET_FILE_DIR:${lib}>")
  else()
    message(DEBUG "did not find library for ${lib} for ${test_names}")
  endif()

endforeach()

list(REMOVE_DUPLICATES dll_mod)

if(dll_mod)
  message(VERBOSE "environment_modification ${dll_mod} for ${test_names}")

  set_property(TEST ${test_names} PROPERTY ENVIRONMENT_MODIFICATION "${dll_mod}")
else()
  message(VERBOSE "no environment_modification for ${test_names}")
endif()


endfunction(dll_test_path)
