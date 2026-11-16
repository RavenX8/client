# expat 2.0.1 (in-tree) as a shared library named xmlparse (xmlparse.dll on Windows,
# which the client ships next to TRose.exe).
set(EXPAT_SOURCE_DIR ${CMAKE_SUBMODULE_DIR}/expat-2.0.1)

set(_expat_sources
  ${EXPAT_SOURCE_DIR}/lib/xmlparse.c
  ${EXPAT_SOURCE_DIR}/lib/xmlrole.c
  ${EXPAT_SOURCE_DIR}/lib/xmltok.c   # #includes xmltok_impl.c / xmltok_ns.c
)
if(WIN32)
  list(APPEND _expat_sources ${EXPAT_SOURCE_DIR}/lib/libexpat.def)
endif()

add_library(expat SHARED ${_expat_sources})
target_include_directories(expat SYSTEM PUBLIC ${EXPAT_SOURCE_DIR}/lib)
set_target_properties(expat PROPERTIES FOLDER 3rdparty OUTPUT_NAME xmlparse)
if(WIN32)
  target_compile_definitions(expat PRIVATE COMPILED_FROM_DSP _WINDOWS _USRDLL EXPAT_EXPORTS _MBCS)
else()
  # what expat_config.h (autoconf) would define; little-endian targets only
  target_compile_definitions(expat PRIVATE HAVE_MEMMOVE XML_NS XML_DTD XML_CONTEXT_BYTES=1024 BYTEORDER=1234)
endif()
if(MSVC)
  target_compile_options(expat PRIVATE /w)
else()
  target_compile_options(expat PRIVATE -w)
endif()

if(NOT TARGET utils::expat)
  add_library(utils::expat ALIAS expat)
endif()

set(EXPAT_INCLUDE_DIR "${EXPAT_SOURCE_DIR}/lib")
set(EXPAT_LIBRARIES expat)
