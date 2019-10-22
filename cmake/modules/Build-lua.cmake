set(LUA_INSTALL_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

set(_byproducts
  ${LUA_INSTALL_DIR}/liblua4.lib
  ${LUA_INSTALL_DIR}/liblua4.a
  ${LUA_INSTALL_DIR}/liblua5.lib
  ${LUA_INSTALL_DIR}/liblua5.a
)

if(WIN32)
  ExternalProject_Add(
    lua4
    DOWNLOAD_COMMAND ""
    SOURCE_DIR ${CMAKE_SUBMODULE_DIR}/lua-4.0.1
    CONFIGURE_COMMAND ""
    BUILD_COMMAND cl /c /I <SOURCE_DIR>/include /EHsc <SOURCE_DIR>/src/lua/lua.c 
    <SOURCE_DIR>/src/lapi.c
    <SOURCE_DIR>/src/lcode.c
    <SOURCE_DIR>/src/ldebug.c
    <SOURCE_DIR>/src/ldo.c
    <SOURCE_DIR>/src/lfunc.c
    <SOURCE_DIR>/src/lgc.c
    <SOURCE_DIR>/src/lib/lauxlib.c
    <SOURCE_DIR>/src/lib/lbaselib.c
    <SOURCE_DIR>/src/lib/ldblib.c
    <SOURCE_DIR>/src/lib/liolib.c
    <SOURCE_DIR>/src/lib/lmathlib.c
    <SOURCE_DIR>/src/lib/lstrlib.c
    <SOURCE_DIR>/src/llex.c
    <SOURCE_DIR>/src/lmem.c
    <SOURCE_DIR>/src/lobject.c
    <SOURCE_DIR>/src/lparser.c
    <SOURCE_DIR>/src/lstate.c
    <SOURCE_DIR>/src/lstring.c
    <SOURCE_DIR>/src/ltable.c
    <SOURCE_DIR>/src/ltests.c
    <SOURCE_DIR>/src/ltm.c
    <SOURCE_DIR>/src/lundump.c
    <SOURCE_DIR>/src/lvm.c
    <SOURCE_DIR>/src/lzio.c
    && lib *.obj /OUT:${LUA_INSTALL_DIR}/liblua4.lib
    INSTALL_COMMAND ""
    BUILD_IN_SOURCE true
    INSTALL_DIR ${LUA_INSTALL_DIR}/
    BUILD_BYPRODUCTS ${_byproducts}
  )
  
  ExternalProject_Add(
    lua5
    DOWNLOAD_COMMAND ""
    SOURCE_DIR ${CMAKE_SUBMODULE_DIR}/lua-5.0.0
    CONFIGURE_COMMAND ""
    BUILD_COMMAND cl /c /I <SOURCE_DIR>/include /EHsc <SOURCE_DIR>/src/lua/lua.c
    <SOURCE_DIR>/src/lapi.c
    <SOURCE_DIR>/src/lcode.c
    <SOURCE_DIR>/src/ldebug.c
    <SOURCE_DIR>/src/ldo.c
    <SOURCE_DIR>/src/ldump.c
    <SOURCE_DIR>/src/lfunc.c
    <SOURCE_DIR>/src/lgc.c
    <SOURCE_DIR>/src/lib/lauxlib.c
    <SOURCE_DIR>/src/lib/lbaselib.c
    <SOURCE_DIR>/src/lib/ldblib.c
    <SOURCE_DIR>/src/lib/liolib.c
    <SOURCE_DIR>/src/lib/lmathlib.c
    <SOURCE_DIR>/src/lib/loadlib.c
    <SOURCE_DIR>/src/lib/lstrlib.c
    <SOURCE_DIR>/src/lib/ltablib.c
    <SOURCE_DIR>/src/llex.c
    <SOURCE_DIR>/src/lmem.c
    <SOURCE_DIR>/src/lobject.c
    <SOURCE_DIR>/src/lopcodes.c
    <SOURCE_DIR>/src/lparser.c
    <SOURCE_DIR>/src/lstate.c
    <SOURCE_DIR>/src/lstring.c
    <SOURCE_DIR>/src/ltable.c
    <SOURCE_DIR>/src/ltests.c
    <SOURCE_DIR>/src/ltm.c
    <SOURCE_DIR>/src/lundump.c
    <SOURCE_DIR>/src/lvm.c
    <SOURCE_DIR>/src/lzio.c
    && lib *.obj /OUT:${LUA_INSTALL_DIR}/liblua5.lib
    INSTALL_COMMAND ""
    BUILD_IN_SOURCE true
    INSTALL_DIR ${LUA_INSTALL_DIR}
    #BUILD_BYPRODUCTS ${_byproducts}
  )
else()
#  find_library(LUA_DL_LIBRARY dl)
#  find_library(LUA_MATH_LIBRARY m)
#  ExternalProject_Add(
#    lua5
#    URL https://www.lua.org/ftp/lua-5.3.4.tar.gz
#    URL_HASH SHA1=79790cfd40e09ba796b01a571d4d63b52b1cd950
#    INSTALL_DIR ${LUA_INSTALL_DIR}
#    BUILD_IN_SOURCE TRUE
#
#    CONFIGURE_COMMAND ""
#    BUILD_COMMAND make linux
#    INSTALL_COMMAND make install INSTALL_TOP=<INSTALL_DIR>
#    BUILD_BYPRODUCTS ${_byproducts}
#  )
endif()

ExternalProject_Get_Property(
  lua4
  source_dir
  install_dir
)
if(WIN32)
  set(LUA_LIBRARY "${install_dir}/liblua4.lib")
  set(LUA_LIBRARIES "${LUA_LIBRARY}")
  set(LUA_INSTALL_LIBS "${install_dir}/liblua.dll")
endif()

set(LUA_INCLUDE_DIR "${source_dir}/include")

if(NOT TARGET lua::lua4)
  add_library(lua::lua4 INTERFACE IMPORTED)
  add_dependencies(lua::lua4 lua4)
  set_target_properties(lua::lua4 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${LUA_INCLUDE_DIR}")
  set_target_properties(lua::lua4 PROPERTIES INTERFACE_LINK_LIBRARIES "${LUA_LIBRARIES}")
endif()

#Lua 5 stoof
ExternalProject_Get_Property(
  lua5
  source_dir
  install_dir
)

if(WIN32)
  set(LUA_LIBRARY "${install_dir}/liblua5.lib")
  set(LUA_LIBRARIES "${LUA_LIBRARY}")
  set(LUA_INSTALL_LIBS "${install_dir}/liblua.dll")
endif()


set(LUA_INCLUDE_DIR "${source_dir}/include")

if(NOT TARGET lua::lua5)
    add_library(lua::lua5 INTERFACE IMPORTED)
    add_dependencies(lua::lua5 lua5)
    set_target_properties(lua::lua5 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${LUA_INCLUDE_DIR}")
    set_target_properties(lua::lua5 PROPERTIES INTERFACE_LINK_LIBRARIES "${LUA_LIBRARIES}")
endif()

#mark_as_advanced(LUA_INCLUDE_DIR LUA_LIBRARY LUA_LIBRARIES)
