function(GENERATE_LUA_INCLUDES HDRS)
  cmake_parse_arguments(ARG "DEBUG;LUA4" "LUAROOT;TARGET" "LUAFILES;OUTPUT" ${ARGN})

  if(NOT ARG_LUAFILES)
    message(SEND_ERROR "Error: GENERATE_LUA_INCLUDES() called without any files")
    return()
  endif(NOT ARG_LUAFILES)
  
  set(SRC_OUTPATH ${ARG_SRC_OUTPATH})
  set(HDR_OUTPATH ${ARG_HDR_OUTPATH})
  set(LUAROOT ${ARG_LUAROOT})
  set(TARGET ${ARG_TARGET})

  if(ARG_DEBUG)
    message("LUAFILES: ${LUAROOT}")
    message("TARGET: ${TARGET}")
  endif()

  set(${HDRS})
  foreach(LUAFILES ${ARG_LUAFILES})
    # ensure that the file ends with .proto
    string(REGEX MATCH "\\.lua$$" PROTOEND ${LUAFILES})
    if(NOT PROTOEND)
      message(SEND_ERROR "lua file '${LUAFILES}' does not end with .lua")
    endif()

    get_filename_component(IDL_PATH ${LUAFILES} PATH)
    get_filename_component(ABS_FILE ${LUAFILES} ABSOLUTE)
    get_filename_component(FILE_WE ${LUAFILES} NAME_WE)

    if(ARG_DEBUG)
      message("file ${LUAFILES}:")
      message("  PATH=${IDL_PATH}")
      message("  ABS_FILE=${ABS_FILE}")
      message("  FILE_WE=${FILE_WE}")
      message("  LUAROOT=${LUAROOT}")
    endif()

    # find out of the file is in the specified proto root
    # TODO clean the LUAROOT so that it does not form a regex itself?
    string(REGEX MATCH "^${LUAROOT}" IN_ROOT_PATH ${LUAFILES})
    string(REGEX MATCH "^${LUAROOT}" IN_ROOT_ABS_FILE ${ABS_FILE})

    if(IN_ROOT_PATH)
      set(MATCH_PATH ${LUAFILES})
    elseif(IN_ROOT_ABS_FILE)
      set(MATCH_PATH ${ABS_FILE})
    else()
      message(SEND_ERROR "LUA file '${LUAFILES}' is not in LUAROOT '${LUAROOT}'")
    endif()

    # build the result file name
    string(REGEX REPLACE "^${LUAROOT}(/?)" "" ROOT_CLEANED_FILE ${MATCH_PATH})
    if(ARG_DEBUG)
      message("  ROOT_CLEANED_FILE=${ROOT_CLEANED_FILE}")
    endif()
    string(REGEX REPLACE "\\.lua$$" "" EXT_CLEANED_FILE ${ROOT_CLEANED_FILE})
    if(ARG_DEBUG)
        message("  EXT_CLEANED_FILE=${EXT_CLEANED_FILE}")
    endif()

    if(NOT ARG_OUTPUT)
      set(H_FILE "${LUAROOT}/${EXT_CLEANED_FILE}.inc")
    else()
      set(H_FILE ${ARG_OUTPUT})
    endif()

    if(ARG_DEBUG)
      message("  MATCH_PATH=${MATCH_PATH}")
      message("  H_FILE=${H_FILE}")
    endif()

    list(APPEND ${HDRS} ${H_FILE})

    if(ARG_LUA4)
      add_custom_command(
        OUTPUT ${H_FILE}
        WORKING_DIRECTORY ${LUAROOT}
        COMMAND lua::lua4_interp "${MATCH_PATH}"
        DEPENDS ${ABS_FILE} lua::lua4_interp
        COMMENT "Running lua4 compiler on ${MATCH_PATH} with root ${LUAROOT}, generating: ${H_FILE}"
        VERBATIM)
     else()
       add_custom_command(
          OUTPUT ${H_FILE}
          WORKING_DIRECTORY ${LUAROOT}
          COMMAND lua::lua5_interp "${MATCH_PATH}"
          DEPENDS ${ABS_FILE} lua::lua5_interp
          COMMENT "Running lua5 compiler on ${MATCH_PATH} with root ${LUAROOT}, generating: ${H_FILE}"
          VERBATIM)
     endif()
  endforeach()
  if(ARG_LUA4)
    add_custom_target(${TARGET}_generated ALL
      DEPENDS lua::lua4_interp ${${HDRS}}
    )
  else()
    add_custom_target(${TARGET}_generated ALL
      DEPENDS lua::lua5_interp ${${HDRS}}
    )
  endif()

  set_source_files_properties(${${HDRS}} PROPERTIES GENERATED TRUE)
  set(${HDRS} ${${HDRS}} PARENT_SCOPE)
endfunction()