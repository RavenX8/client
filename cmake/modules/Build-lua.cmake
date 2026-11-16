# Lua 5.4.3 (in-tree) as regular targets:
#   lua5_core / lua5_std  static libraries (linked through lua::lua5)
#   lua5_interp           lua5 interpreter, run at build time by LuaIncludeGenerator
#   lua5_compiler         luac5
# When cross-compiling, build-time runs of lua5_interp go through
# CMAKE_CROSSCOMPILING_EMULATOR (e.g. wine) automatically.
set(LUA_SOURCE_DIR ${CMAKE_SUBMODULE_DIR}/lua-5.4.3)
set(_lua_src ${LUA_SOURCE_DIR}/src)

add_library(lua5_core STATIC
  ${_lua_src}/lapi.c ${_lua_src}/lcode.c ${_lua_src}/lctype.c ${_lua_src}/ldebug.c
  ${_lua_src}/ldo.c ${_lua_src}/ldump.c ${_lua_src}/lfunc.c ${_lua_src}/lgc.c
  ${_lua_src}/llex.c ${_lua_src}/lmem.c ${_lua_src}/lobject.c ${_lua_src}/lopcodes.c
  ${_lua_src}/lparser.c ${_lua_src}/lstate.c ${_lua_src}/lstring.c ${_lua_src}/ltable.c
  ${_lua_src}/ltm.c ${_lua_src}/lundump.c ${_lua_src}/lvm.c ${_lua_src}/lzio.c
)
set_target_properties(lua5_core PROPERTIES OUTPUT_NAME liblua5)

add_library(lua5_std STATIC
  ${_lua_src}/lauxlib.c ${_lua_src}/lbaselib.c ${_lua_src}/lcorolib.c ${_lua_src}/ldblib.c
  ${_lua_src}/liolib.c ${_lua_src}/lmathlib.c ${_lua_src}/loadlib.c ${_lua_src}/loslib.c
  ${_lua_src}/lstrlib.c ${_lua_src}/ltablib.c ${_lua_src}/lutf8lib.c ${_lua_src}/linit.c
)
set_target_properties(lua5_std PROPERTIES OUTPUT_NAME liblua5lib)
target_link_libraries(lua5_std PUBLIC lua5_core)

foreach(_t lua5_core lua5_std)
  target_include_directories(${_t} SYSTEM PUBLIC ${_lua_src})
  set_target_properties(${_t} PROPERTIES FOLDER 3rdparty POSITION_INDEPENDENT_CODE ON)
  if(MSVC)
    target_compile_options(${_t} PRIVATE /w)
  else()
    target_compile_options(${_t} PRIVATE -w)
    target_compile_definitions(${_t} PRIVATE LUA_USE_LINUX)
  endif()
endforeach()
if(NOT WIN32)
  target_link_libraries(lua5_core PUBLIC m ${CMAKE_DL_LIBS})
endif()

add_executable(lua5_interp ${_lua_src}/lua.c)
add_executable(lua5_compiler ${_lua_src}/luac.c)
foreach(_t lua5_interp lua5_compiler)
  target_link_libraries(${_t} PRIVATE lua5_std)
  target_compile_definitions(${_t} PRIVATE LUA_OPNAMES)
  set_target_properties(${_t} PROPERTIES FOLDER 3rdparty RUNTIME_OUTPUT_DIRECTORY ${CMAKE_THIRD_PARTY_BINARY_DIR})
endforeach()
set_target_properties(lua5_interp PROPERTIES OUTPUT_NAME lua5)
set_target_properties(lua5_compiler PROPERTIES OUTPUT_NAME luac5)

if(NOT TARGET lua::lua5)
  add_library(lua::lua5 ALIAS lua5_std)
endif()

set(LUA_INCLUDE_DIR "${_lua_src}")
set(LUA_LIBRARIES lua5_std lua5_core)
