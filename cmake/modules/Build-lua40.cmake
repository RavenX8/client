# Lua 4.0.1 (in-tree), only for the game's event (.con) scripts, which are
# Lua 4.0 bytecode. Every symbol is renamed (lua40_names.h, force-included)
# so it links beside Lua 5.4; its loader reads chunks compiled by a 32-bit
# Lua (lundump.c, size_t of 4 bytes).
set(LUA40_DIR ${CMAKE_SUBMODULE_DIR}/lua-4.0.1)
file(GLOB _lua40_sources ${LUA40_DIR}/src/*.c ${LUA40_DIR}/src/lib/*.c)

add_library(lua40 STATIC ${_lua40_sources})
target_include_directories(lua40 PRIVATE ${LUA40_DIR}/src ${LUA40_DIR}/include)
# Users include "lua-4.0.1/include/lua.h" by that path: the bare "lua.h" is 5.4's.
target_include_directories(lua40 INTERFACE ${CMAKE_SUBMODULE_DIR})
set_target_properties(lua40 PROPERTIES FOLDER 3rdparty POSITION_INDEPENDENT_CODE ON)
if(MSVC)
  target_compile_options(lua40 PRIVATE /w /FI${LUA40_DIR}/include/lua40_names.h)
else()
  target_compile_options(lua40 PRIVATE -w -include ${LUA40_DIR}/include/lua40_names.h)
endif()
