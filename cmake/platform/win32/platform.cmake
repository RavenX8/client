
add_compiler_flags(-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)

if("${CMAKE_GENERATOR}" MATCHES "Ninja")
  set(NINJA TRUE)
elseif("${CMAKE_GENERATOR}" MATCHES "Make")
  set(MAKEFILES TRUE)
elseif("${CMAKE_GENERATOR}" MATCHES "Visual Studio")
  set(MSBUILD TRUE)
endif()

set(COPY_SCRIPT_PATH ${CMAKE_SCRIPT_PATH}/robocopy.bat)

if(MAKEFILES AND "${BUILD_TYPE}" STREQUAL "Release")
  set(OFFICIAL_BUILD ON CACHE BOOL "" FORCE)
  set(BUILD_TOOLS OFF CACHE BOOL "" FORCE)
  set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(WITH_GTEST OFF CACHE BOOL "" FORCE )
  set(WITH_GMOCK OFF CACHE BOOL "" FORCE )
  set(DEBUG OFF CACHE BOOL "" FORCE)
endif()

macro(use_unicode_here)
  add_definitions(-D_UNICODE -DUNICODE)
endmacro()

add_definitions(-DWIN32)
add_linker_flags(/SUBSYSTEM:WINDOWS,5.1)

if(DEBUG)
  add_definitions(-DDEBUG -D_DEBUG)
else()
  add_definitions(-DNDEBUG -D_NDEBUG)
  
  add_compile_options(/Zi)
  add_linker_flags(/DEBUG /OPT:REF /OPT:ICF)
endif()

add_definitions(-DNOMINMAX)

# ignore some warnings
add_linker_flags(/ignore:4075)
add_compiler_flags(/wd4503)
add_compiler_flags(/EHsc)

set(WIN_TARGET 0x0601) # Windows XP SP2
set(WIN_SDK_MIN 0x0601) # Windows Vista
add_definitions(-DWINVER=${WIN_TARGET}
                -D_WIN32_WINNT=${WIN_SDK_MIN})

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(64BIT TRUE)
  message("-- detected 64bit")
else()
  set(64BIT FALSE)
  message("-- detected 32bit")
endif()

if(64BIT)
  set(BUILD_PLATFORM "x64")
else()
  set(BUILD_PLATFORM "x86")
  add_linker_flags(/LARGEADDRESSAWARE)
endif()

macro(SetSharedRuntime target)
  if(DEBUG)
    set_target_properties(${target} PROPERTIES COMPILE_FLAGS "/MDd")
  else()
    set_target_properties(${target} PROPERTIES COMPILE_FLAGS "/MD")
  endif()
endmacro()

macro(SetStaticRuntime target)
  if(DEBUG)
    set_target_properties(${target} PROPERTIES COMPILE_FLAGS "/MTd")
  else()
    set_target_properties(${target} PROPERTIES COMPILE_FLAGS "/MT")
  endif()
endmacro()

set(MSVC TRUE)
