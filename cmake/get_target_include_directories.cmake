function(get_target_include_directories VAR)
  set(options)
  set(oneValueArgs TARGET)
  set(multiValueArgs)
  cmake_parse_arguments(ARGS
    "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if (TARGET ${ARGS_TARGET})
    # message("${ARGS_TARGET}")
    get_target_property(IS_IMPORTED ${ARGS_TARGET} IMPORTED)
    # message("imported: ${IS_IMPORTED}")
    if (${IS_IMPORTED})
      # get_target_property(INCLUDE_DIRECTORIES ${ARGS_TARGET} INTERFACE_INCLUDE_DIRECTORIES)
    else (${IS_IMPORTED})
      get_target_property(INCLUDE_DIRECTORIES ${ARGS_TARGET} INCLUDE_DIRECTORIES)
      get_target_property(LINK_LIBRARIES ${ARGS_TARGET} LINK_LIBRARIES)

      foreach(LIB ${LINK_LIBRARIES})
        get_target_include_directories(LIB_INCLUDE_DIRECTORIES TARGET ${LIB})
        if (${LIB_INCLUDE_DIRECTORIES})
          set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} ${LIB_INCLUDE_DIRECTORIES})
        endif (${LIB_INCLUDE_DIRECTORIES})
      endforeach(LIB ${LINK_LIBRARIES})
    endif(${IS_IMPORTED})

  else (TARGET ${ARGS_TARGET})
    set(INCLUDE_DIRECTORIES "")
  endif (TARGET ${ARGS_TARGET})

  if (${INCLUDE_DIRECTORIES})
    set(${VAR} ${INCLUDE_DIRECTORIES} PARENT_SCOPE)
  endif (${INCLUDE_DIRECTORIES})

endfunction(get_target_include_directories)