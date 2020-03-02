set(LUA_INSTALL_DIR ${CMAKE_THIRD_PARTY_LIBRARY_DIR})

if(WIN32)
  ExternalProject_Add(
    lua4_core
    DOWNLOAD_COMMAND ""
    SOURCE_DIR ${CMAKE_SUBMODULE_DIR}/lua-4.0.1
    CONFIGURE_COMMAND ""
    BUILD_COMMAND cl /c /I <SOURCE_DIR>/include /I <SOURCE_DIR>/src /EHsc
    <SOURCE_DIR>/src/lapi.c
    <SOURCE_DIR>/src/lcode.c
    <SOURCE_DIR>/src/ldebug.c
    <SOURCE_DIR>/src/ldo.c
    <SOURCE_DIR>/src/lfunc.c
    <SOURCE_DIR>/src/lgc.c
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
    && lib lapi.obj lcode.obj ldebug.obj ldo.obj lfunc.obj lgc.obj llex.obj lmem.obj lobject.obj lparser.obj lstate.obj lstring.obj ltable.obj ltests.obj ltm.obj lundump.obj lvm.obj lzio.obj
    /OUT:${LUA_INSTALL_DIR}/liblua4.lib
    INSTALL_COMMAND ""
    BUILD_IN_SOURCE true
    INSTALL_DIR ${LUA_INSTALL_DIR}
    BUILD_BYPRODUCTS ${LUA_INSTALL_DIR}/liblua4.lib ${LUA_INSTALL_DIR}/liblua4.a
  )
  
  ExternalProject_Add(
    lua4_std
    DOWNLOAD_COMMAND ""
    SOURCE_DIR ${CMAKE_SUBMODULE_DIR}/lua-4.0.1
    CONFIGURE_COMMAND ""
    BUILD_COMMAND cl /c /I <SOURCE_DIR>/include /I <SOURCE_DIR>/src /EHsc
    <SOURCE_DIR>/src/lib/lauxlib.c
    <SOURCE_DIR>/src/lib/lbaselib.c
    <SOURCE_DIR>/src/lib/ldblib.c
    <SOURCE_DIR>/src/lib/liolib.c
    <SOURCE_DIR>/src/lib/lmathlib.c
    <SOURCE_DIR>/src/lib/lstrlib.c
    && lib lauxlib.obj lbaselib.obj ldblib.obj liolib.obj lmathlib.obj lstrlib.obj /OUT:${LUA_INSTALL_DIR}/liblua4lib.lib
    INSTALL_COMMAND ""
    BUILD_IN_SOURCE true
    INSTALL_DIR ${LUA_INSTALL_DIR}
    BUILD_BYPRODUCTS ${LUA_INSTALL_DIR}/liblua4lib.lib
  )
  
  ExternalProject_Add(
    lua4_interp
    DEPENDS lua4_core lua4_std
    DOWNLOAD_COMMAND ""
    SOURCE_DIR ${CMAKE_SUBMODULE_DIR}/lua-4.0.1
    CONFIGURE_COMMAND ""
    BUILD_COMMAND cl /I <SOURCE_DIR>/include /I <SOURCE_DIR>/src /EHsc
    <SOURCE_DIR>/src/lua/lua.c
    ${LUA_INSTALL_DIR}/liblua4.lib
    ${LUA_INSTALL_DIR}/liblua4lib.lib
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/lua.exe <INSTALL_DIR>/lua4.exe
    BUILD_IN_SOURCE true
    INSTALL_DIR ${CMAKE_THIRD_PARTY_BINARY_DIR}
    BUILD_BYPRODUCTS ${CMAKE_THIRD_PARTY_BINARY_DIR}/lua4.exe
  )
  
  ExternalProject_Add(
    lua4_compiler
    DEPENDS lua4_core lua4_std
    DOWNLOAD_COMMAND ""
    SOURCE_DIR ${CMAKE_SUBMODULE_DIR}/lua-4.0.1
    CONFIGURE_COMMAND ""
    BUILD_COMMAND cl /I <SOURCE_DIR>/include /I <SOURCE_DIR>/src /I <SOURCE_DIR>/src/luac /EHsc
    <SOURCE_DIR>/src/luac/luac.c
    <SOURCE_DIR>/src/luac/dump.c
    <SOURCE_DIR>/src/luac/opt.c
    <SOURCE_DIR>/src/luac/stubs.c
    <SOURCE_DIR>/src/luac/print.c
    ${LUA_INSTALL_DIR}/liblua4.lib
    ${LUA_INSTALL_DIR}/liblua4lib.lib
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/luac.exe <INSTALL_DIR>/luac4.exe
    BUILD_IN_SOURCE true
    INSTALL_DIR ${CMAKE_THIRD_PARTY_BINARY_DIR}
    BUILD_BYPRODUCTS ${CMAKE_THIRD_PARTY_BINARY_DIR}/luac4.exe
  )
  
  ExternalProject_Add(
    lua5_core
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
    && lib lapi.obj lcode.obj ldebug.obj ldo.obj ldump.obj lfunc.obj lgc.obj llex.obj lmem.obj lobject.obj lopcodes.obj lparser.obj lstate.obj lstring.obj ltable.obj ltests.obj ltm.obj lundump.obj lvm.obj lzio.obj
    /OUT:${LUA_INSTALL_DIR}/liblua5.lib
    INSTALL_COMMAND ""
    BUILD_IN_SOURCE true
    INSTALL_DIR ${LUA_INSTALL_DIR}
    BUILD_BYPRODUCTS ${LUA_INSTALL_DIR}/liblua5.lib ${LUA_INSTALL_DIR}/liblua5.a
  )
  
  ExternalProject_Add(
    lua5_std
    DOWNLOAD_COMMAND ""
    SOURCE_DIR ${CMAKE_SUBMODULE_DIR}/lua-5.0.0
    CONFIGURE_COMMAND ""
    BUILD_COMMAND cl /c /I <SOURCE_DIR>/include /I <SOURCE_DIR>/src /EHsc
    <SOURCE_DIR>/src/lib/lauxlib.c
    <SOURCE_DIR>/src/lib/lbaselib.c
    <SOURCE_DIR>/src/lib/ldblib.c
    <SOURCE_DIR>/src/lib/liolib.c
    <SOURCE_DIR>/src/lib/lmathlib.c
    <SOURCE_DIR>/src/lib/ltablib.c
    <SOURCE_DIR>/src/lib/lstrlib.c
    <SOURCE_DIR>/src/lib/loadlib.c
    && lib lauxlib.obj lbaselib.obj ldblib.obj liolib.obj lmathlib.obj ltablib.obj lstrlib.obj loadlib.obj /OUT:${LUA_INSTALL_DIR}/liblua5lib.lib
    INSTALL_COMMAND ""
    BUILD_IN_SOURCE true
    INSTALL_DIR ${LUA_INSTALL_DIR}
    BUILD_BYPRODUCTS ${LUA_INSTALL_DIR}/liblua5lib.lib
  )
  
  ExternalProject_Add(
    lua5_interp
    DEPENDS lua5_core lua5_std
    DOWNLOAD_COMMAND ""
    SOURCE_DIR ${CMAKE_SUBMODULE_DIR}/lua-5.0.0
    CONFIGURE_COMMAND ""
    BUILD_COMMAND cl /I <SOURCE_DIR>/include /I <SOURCE_DIR>/src/EHsc -DLUA_OPNAMES
    <SOURCE_DIR>/src/lua/lua.c
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
    SOURCE_DIR ${CMAKE_SUBMODULE_DIR}/lua-5.0.0
    CONFIGURE_COMMAND ""
    BUILD_COMMAND cl /I <SOURCE_DIR>/include /I <SOURCE_DIR>/src /I <SOURCE_DIR>/src/luac /EHsc -DLUA_OPNAMES
    <SOURCE_DIR>/src/luac/luac.c
    <SOURCE_DIR>/src/luac/print.c
    <SOURCE_DIR>/src/lopcodes.c
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

ExternalProject_Get_Property(
  lua4_core
  source_dir
  install_dir
)
if(WIN32)
  set(LUA_LIBRARY "${install_dir}/liblua4.lib")
  set(LUA_LIBRARIES "${LUA_LIBRARY}" "${install_dir}/liblua4lib.lib")
  set(LUA_INSTALL_LIBS "${install_dir}/liblua.dll")
endif()

set(LUA_INCLUDE_DIR "${source_dir}/include")

if(NOT TARGET lua::lua4)
  add_library(lua::lua4 INTERFACE IMPORTED)
  add_dependencies(lua::lua4 lua4_core lua4_std)
  set_target_properties(lua::lua4 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${LUA_INCLUDE_DIR}")
  set_target_properties(lua::lua4 PROPERTIES INTERFACE_LINK_LIBRARIES "${LUA_LIBRARIES}")
endif()

if(NOT TARGET lua::lua4_interp)
  add_executable(lua::lua4_interp IMPORTED)
  add_dependencies(lua::lua4_interp lua4_interp)
  set_target_properties(lua::lua4_interp PROPERTIES IMPORTED_LOCATION "${CMAKE_THIRD_PARTY_BINARY_DIR}/lua4.exe")
endif()

if(NOT TARGET lua::lua4_compiler)
  add_executable(lua::lua4_compiler IMPORTED)
  add_dependencies(lua::lua4_compiler lua4_compiler)
  set_target_properties(lua::lua4_compiler PROPERTIES IMPORTED_LOCATION "${CMAKE_THIRD_PARTY_BINARY_DIR}/luac4.exe")
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


set(LUA_INCLUDE_DIR "${source_dir}/include")

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
