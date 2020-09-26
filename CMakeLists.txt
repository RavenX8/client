cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project("Client")

#set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)

if(CMAKE_CXX_COMPILER_LAUNCHER)
  set(ENV{CCACHE_SLOPPINESS} "pch_defines,time_macros")
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
set(CMAKE_PLATFORM_PATH "${CMAKE_SOURCE_DIR}/cmake/platform")
set(CMAKE_SCRIPT_PATH "${CMAKE_SOURCE_DIR}/cmake/scripts")
set(CMAKE_PATCH_DIR "${CMAKE_SOURCE_DIR}/cmake/patches")
set(CMAKE_PREFIX_PATH "${CMAKE_THIRD_PARTY_DIR}")
set(CMAKE_SUBMODULE_DIR "${CMAKE_SOURCE_DIR}/3rdparty")

set(CMAKE_THIRD_PARTY_DIR "${CMAKE_BINARY_DIR}/3rdparty")
set(CMAKE_THIRD_PARTY_LIBRARY_DIR "${CMAKE_THIRD_PARTY_DIR}/lib")
set(CMAKE_THIRD_PARTY_INCLUDE_DIR "${CMAKE_THIRD_PARTY_DIR}")
set(CMAKE_GEN_SRC_DIR "${CMAKE_BINARY_DIR}/gen")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(DIRECTX_ROOT ${CMAKE_SUBMODULE_DIR}/dx90csdk)
set(SPDLOG_ROOT ${CMAKE_SUBMODULE_DIR}/spdlog/include)
set(LUA_DIR ${CMAKE_SUBMODULE_DIR}/lua-5.0.0)

include(CMakeParseArguments)
include(GNUInstallDirs)
include(ExternalProject)
include(CheckOptions)

if(NOT CMAKE_BUILD_TYPE)
  if(OFFICIAL_BUILD)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
  elseif(DEBUG)
    set(CMAKE_BUILD_TYPE Debug)
  else()
    set(CMAKE_BUILD_TYPE Release)
  endif()

  if(OFFICIAL_DEBUG_BUILD)
    set(CMAKE_BUILD_TYPE Debug)
  endif()
endif()

set(BUILD_TYPE "${CMAKE_BUILD_TYPE}")

if("${CMAKE_GENERATOR}" MATCHES "Visual Studio")
  set(BUILD_TYPE "$<CONFIG>")
endif()

set(CMAKE_THIRD_PARTY_BINARY_DIR ${CMAKE_THIRD_PARTY_DIR}/bin)
set(CMAKE_THIRD_PARTY_LIBRARY_DIR ${CMAKE_THIRD_PARTY_DIR}/lib)
set(CMAKE_THIRD_PARTY_INCLUDE_DIR ${CMAKE_THIRD_PARTY_DIR}/include)
set(CMAKE_PREFIX_PATH "${CMAKE_THIRD_PARTY_DIR}")

# Create folders
file(MAKE_DIRECTORY ${CMAKE_THIRD_PARTY_BINARY_DIR})
file(MAKE_DIRECTORY ${CMAKE_THIRD_PARTY_INCLUDE_DIR})
file(MAKE_DIRECTORY ${CMAKE_THIRD_PARTY_LIBRARY_DIR})
file(MAKE_DIRECTORY ${CMAKE_THIRD_PARTY_DIR}/share)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include(macros)
include(CheckPlatform)
include(SetInstallPaths)
include(cotire)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Patch REQUIRED)
if(MSVC)
  configure_file(${CMAKE_SCRIPT_PATH}/patch.bat ${CMAKE_GEN_SRC_DIR}/scripts/patch.bat @ONLY)
  set(PATCH_SCRIPT_PATH ${CMAKE_GEN_SRC_DIR}/scripts/patch.bat)
else()
  set(PATCH_SCRIPT_PATH ${CMAKE_SCRIPT_PATH}/patch.sh)
endif()

find_package(Threads REQUIRED)

include(Build-ogg)
include(Build-lua)
include(Build-zlib)
include(Build-expat)
include(Build-ijl15) # This doesn't actually build anything, just sets up the target
include(Build-sqlite3)
include(Build-vorbis)
#include(Build-Ultralight)

include(LuaIncludeGenerator)

include_directories(${CMAKE_GEN_SRC_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS")

#add_subdirectory(3rdparty)
add_subdirectory(src)

#--------------------------------
# Build API Docs
#--------------------------------
if (BUILD_API_DOC)
  find_package(Doxygen REQUIRED)
  if (DOXYGEN_FOUND)
    add_subdirectory(doc)
  endif (DOXYGEN_FOUND)
endif (BUILD_API_DOC)
