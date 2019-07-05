set(ZLIB_INSTALL_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

set(_byproducts
  ${ZLIB_INSTALL_DIR}/zlib.lib
  ${ZLIB_INSTALL_DIR}/zlib.a
)

if(WIN32 OR MINGW)
  if(MINGW)
    ExternalProject_Add(
      zlib
      DOWNLOAD_COMMAND ""
      SOURCE_DIR ${CMAKE_THIRD_PARTY_DIR}/zlib-1.2.3
      CONFIGURE_COMMAND ""
      BUILD_COMMAND gcc -shared 
      <SOURCE_DIR>/adler32.c
      <SOURCE_DIR>/compress.c
      <SOURCE_DIR>/crc32.c
      <SOURCE_DIR>/deflate.c
      <SOURCE_DIR>/gzio.c
      <SOURCE_DIR>/inffast.c
      <SOURCE_DIR>/inflate.c
      <SOURCE_DIR>/inftrees.c
      <SOURCE_DIR>/trees.c
      <SOURCE_DIR>/uncompr.c
      <SOURCE_DIR>/zutil.c -o ${ZLIB_INSTALL_DIR}/zlib.a
      BUILD_IN_SOURCE true
      BUILD_BYPRODUCTS ${_byproducts}
      INSTALL_COMMAND ""
      INSTALL_DIR ${ZLIB_INSTALL_DIR}
    )
  else()
    ExternalProject_Add(
      zlib
      DOWNLOAD_COMMAND ""
      SOURCE_DIR ${CMAKE_THIRD_PARTY_DIR}/zlib-1.2.3
      CONFIGURE_COMMAND ""
      BUILD_COMMAND cl /c /EHsc /MT 
      <SOURCE_DIR>/adler32.c
      <SOURCE_DIR>/compress.c
      <SOURCE_DIR>/crc32.c
      <SOURCE_DIR>/deflate.c
      <SOURCE_DIR>/gzio.c
      <SOURCE_DIR>/inffast.c
      <SOURCE_DIR>/inflate.c
      <SOURCE_DIR>/inftrees.c
      <SOURCE_DIR>/trees.c
      <SOURCE_DIR>/uncompr.c
      <SOURCE_DIR>/zutil.c
      && lib *.obj /OUT:${ZLIB_INSTALL_DIR}/zlib.lib
      BUILD_IN_SOURCE true
      BUILD_BYPRODUCTS ${_byproducts}
      INSTALL_COMMAND ""
      INSTALL_DIR ${ZLIB_INSTALL_DIR}
    )
  endif()
endif()


ExternalProject_Get_Property(
  zlib
  source_dir
  install_dir
)

set(ZLIB_INCLUDE_DIR "${source_dir}")
if(NOT MINGW AND WIN32)
  set(ZLIB_LIBRARY_DIR "${install_dir}")
  set(ZLIB_LIBRARY "${install_dir}/zlib.lib")
  set(ZLIB_LIBRARIES "${install_dir}/zlib.lib" "${ZLIB_LIBRARIES}")
else()
  set(ZLIB_LIBRARY_DIR "${install_dir}")
  set(ZLIB_LIBRARY "${install_dir}/zlib.a")
  set(ZLIB_LIBRARIES "${install_dir}/zlib.a" "${ZLIB_LIBRARIES}")
endif()

if(NOT TARGET utils::zlib)
  add_library(utils::zlib INTERFACE IMPORTED)
  add_dependencies(utils::zlib zlib)
  set_target_properties(utils::zlib PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${ZLIB_INCLUDE_DIR}")
  set_target_properties(utils::zlib PROPERTIES INTERFACE_LINK_LIBRARIES "${ZLIB_LIBRARIES}")
endif()

mark_as_advanced( ZLIB_INCLUDE_DIR ZLIB_LIBRARIES ZLIB_LIBRARY ZLIB_LIBRARY_PATH )