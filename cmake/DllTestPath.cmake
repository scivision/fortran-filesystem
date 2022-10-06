function(locate_dll loc dll_mod)

foreach(l IN LISTS loc)
  cmake_path(GET l PARENT_PATH lp)
  if(NOT lp)
    # empty generator expression ${l}
    return()
  endif()

  foreach(dl IN ITEMS ${lp} ${lp}/Release ${lp}/Debug)
    message(DEBUG "Looking for ${dll_mod} in ${dl} from ${l} and ${lp}")
    if(IS_DIRECTORY ${dl})
      list(APPEND dll_mod "PATH=path_list_append:${dl}")
      set(${dll_mod} ${${dll_mod}} PARENT_SCOPE)

      cmake_path(SET d NORMALIZE ${dl}/../bin)
      # can't check bin/stem.dll as some libs add arbitrary stuff to stem
      if(IS_DIRECTORY ${d})
        list(APPEND dll_mod "PATH=path_list_append:${d}")
        set(${dll_mod} ${${dll_mod}} PARENT_SCOPE)
      endif()
    endif()
  endforeach()
endforeach()

endfunction(locate_dll)


function(dll_test_path libs test_names)
# if shared lib on Windows, need DLL on PATH

if(NOT WIN32 OR CMAKE_VERSION VERSION_LESS 3.22)
  return()
endif()


set(dll_mod)

foreach(lib IN LISTS libs)

  if(EXISTS ${lib})
    message(DEBUG "${lib} exists as a file")
    list(APPEND dll_mod "PATH=path_list_append:$<TARGET_FILE_DIR:${lib}>")
    continue()
  endif()

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

  # do not use LOCATION property, will error CMake config
  foreach(t IMPORTED_LOCATION IMPORTED_LOCATION_RELEASE IMPORTED_LOCATION_RELWITHDEBINFO IMPORTED_LOCATION_DEBUG IMPORTED_LOCATION_NOCONFIG)
    get_target_property(imploc ${lib} ${t})
    if(imploc)
      message(DEBUG "${lib} ${t} is ${imploc}")
      locate_dll(${imploc} dll_mod)
    endif()
  endforeach()

  get_target_property(intloc ${lib} INTERFACE_LINK_LIBRARIES)

  if(intloc)
    message(DEBUG "${lib} INTERFACE_LINK_LIBRARIES is ${intloc}")
    locate_dll(${intloc} dll_mod)
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
