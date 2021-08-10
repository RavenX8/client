set(LUA_INSTALL_DIR ${CMAKE_THIRD_PARTY_LIBRARY_DIR})

if(WIN32)
  ExternalProject_Add(
    lua5_core
    DOWNLOAD_COMMAND ""
    SOURCE_DIR ${CMAKE_SUBMODULE_DIR}/lua-5.4.3
    CONFIGURE_COMMAND ""
    BUILD_COMMAND cl /c /I <SOURCE_DIR>/include /EHsc
    <SOURCE_DIR>/src/lapi.c
    <SOURCE_DIR>/src/lcode.c
    <SOURCE_DIR>/src/lctype.c
    <SOURCE_DIR>/src/ldebug.c
    <SOURCE_DIR>/src/ldo.c
    <SOURCE_DIR>/src/ldump.c
    <SOURCE_DIR>/src/lfunc.c
    <SOURCE_DIR>/src/lgc.c
    <SOURCE_DIR>/src/llex.c
    <SOURCE_DIR>/src/lmem.c
    <SOURCE_DIR>/src/lobject.c
    <SOURCE_DIR>/src/lopcodes.c
    <SOURCE_DIR>/src/lparser.c
    <SOURCE_DIR>/src/lstate.c
    <SOURCE_DIR>/src/lstring.c
    <SOURCE_DIR>/src/ltable.c
    <SOURCE_DIR>/src/ltm.c
    <SOURCE_DIR>/src/lundump.c
    <SOURCE_DIR>/src/lvm.c
    <SOURCE_DIR>/src/lzio.c
    && lib lapi.obj lcode.obj lctype.obj ldebug.obj ldo.obj ldump.obj lfunc.obj lgc.obj llex.obj lmem.obj lobject.obj lopcodes.obj lparser.obj lstate.obj lstring.obj ltable.obj ltm.obj lundump.obj lvm.obj lzio.obj
    /OUT:${LUA_INSTALL_DIR}/liblua5.lib
    INSTALL_COMMAND ""
    BUILD_IN_SOURCE true
    INSTALL_DIR ${LUA_INSTALL_DIR}
    BUILD_BYPRODUCTS ${LUA_INSTALL_DIR}/liblua5.lib ${LUA_INSTALL_DIR}/liblua5.a
  )
  
  ExternalProject_Add(
    lua5_std
    DOWNLOAD_COMMAND ""
    SOURCE_DIR ${CMAKE_SUBMODULE_DIR}/lua-5.4.3
    CONFIGURE_COMMAND ""
    BUILD_COMMAND cl /c /I <SOURCE_DIR>/src /EHsc
    <SOURCE_DIR>/src/lauxlib.c
    <SOURCE_DIR>/src/lbaselib.c
    <SOURCE_DIR>/src/lcorolib.c
    <SOURCE_DIR>/src/ldblib.c
    <SOURCE_DIR>/src/liolib.c
    <SOURCE_DIR>/src/lmathlib.c
    <SOURCE_DIR>/src/loadlib.c
    <SOURCE_DIR>/src/loslib.c
    <SOURCE_DIR>/src/lstrlib.c
    <SOURCE_DIR>/src/ltablib.c
    <SOURCE_DIR>/src/lutf8lib.c
    <SOURCE_DIR>/src/linit.c
    && lib lauxlib.obj lbaselib.obj lcorolib.obj ldblib.obj liolib.obj lmathlib.obj ltablib.obj lstrlib.obj loadlib.obj loslib.obj lutf8lib.obj linit.obj /OUT:${LUA_INSTALL_DIR}/liblua5lib.lib
    INSTALL_COMMAND ""
    BUILD_IN_SOURCE true
    INSTALL_DIR ${LUA_INSTALL_DIR}
    BUILD_BYPRODUCTS ${LUA_INSTALL_DIR}/liblua5lib.lib
  )
  
  ExternalProject_Add(
    lua5_interp
    DEPENDS lua5_core lua5_std
    DOWNLOAD_COMMAND ""
    SOURCE_DIR ${CMAKE_SUBMODULE_DIR}/lua-5.4.3
    CONFIGURE_COMMAND ""
    BUILD_COMMAND cl /I <SOURCE_DIR>/src/EHsc -DLUA_OPNAMES
    <SOURCE_DIR>/src/lua.c
    ${LUA_INSTALL_DIR}/liblua5.lib
    ${LUA_INSTALL_DIR}/liblua5lib.lib
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/lua.exe <INSTALL_DIR>/lua5.exe
    BUILD_IN_SOURCE true
    INSTALL_DIR ${CMAKE_THIRD_PARTY_BINARY_DIR}
    BUILD_BYPRODUCTS ${CMAKE_THIRD_PARTY_BINARY_DIR}/lua5.exe
  )
  
  ExternalProject_Add(
    lua5_compiler
    DEPENDS lua5_core lua5_std
    DOWNLOAD_COMMAND ""
    SOURCE_DIR ${CMAKE_SUBMODULE_DIR}/lua-5.4.3
    CONFIGURE_COMMAND ""
    BUILD_COMMAND cl /I <SOURCE_DIR>/src /EHsc -DLUA_OPNAMES
    <SOURCE_DIR>/src/luac.c
    ${LUA_INSTALL_DIR}/liblua5.lib
    ${LUA_INSTALL_DIR}/liblua5lib.lib
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/luac.exe <INSTALL_DIR>/luac5.exe
    BUILD_IN_SOURCE true
    INSTALL_DIR ${CMAKE_THIRD_PARTY_BINARY_DIR}
    BUILD_BYPRODUCTS ${CMAKE_THIRD_PARTY_BINARY_DIR}/luac5.exe
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

#Lua 5 stoof
ExternalProject_Get_Property(
  lua5_core
  source_dir
  install_dir
)

if(WIN32)
  set(LUA_LIBRARY "${install_dir}/liblua5.lib")
  set(LUA_LIBRARIES "${LUA_LIBRARY}" "${install_dir}/liblua5lib.lib")
  set(LUA_INSTALL_LIBS "${install_dir}/liblua.dll")
endif()


set(LUA_INCLUDE_DIR "${source_dir}/src")

if(NOT TARGET lua::lua5)
  add_library(lua::lua5 INTERFACE IMPORTED)
  add_dependencies(lua::lua5 lua5_core lua5_std)
  set_target_properties(lua::lua5 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${LUA_INCLUDE_DIR}")
  set_target_properties(lua::lua5 PROPERTIES INTERFACE_LINK_LIBRARIES "${LUA_LIBRARIES}")
endif()

if(NOT TARGET lua::lua5_interp)
  add_executable(lua::lua5_interp IMPORTED)
  add_dependencies(lua::lua5_interp lua5_interp)
  set_target_properties(lua::lua5_interp PROPERTIES IMPORTED_LOCATION "${CMAKE_THIRD_PARTY_BINARY_DIR}/lua5.exe")
 endif()

if(NOT TARGET lua::lua5_compiler)
  add_executable(lua::lua5_compiler IMPORTED)
  add_dependencies(lua::lua5_compiler lua5_compiler)
  set_target_properties(lua::lua5_compiler PROPERTIES IMPORTED_LOCATION "${CMAKE_THIRD_PARTY_BINARY_DIR}/luac5.exe")
 endif()

#mark_as_advanced(LUA_INCLUDE_DIR LUA_LIBRARY LUA_LIBRARIES)
