set(VORBIS_INSTALL_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

set(_byproducts
  ${VORBIS_INSTALL_DIR}/libvorbis.lib
  ${VORBIS_INSTALL_DIR}/libvorbis.a
)

if(WIN32 OR MINGW)
  if(MINGW)
    ExternalProject_Add(
      vorbis
      DOWNLOAD_COMMAND ""
      SOURCE_DIR ${CMAKE_THIRD_PARTY_DIR}/libvorbis-1.2.0
      CONFIGURE_COMMAND ""
      BUILD_COMMAND gcc -shared <SOURCE_DIR>/lib/analysis.c
        <SOURCE_DIR>/lib/bitrate.c
        <SOURCE_DIR>/lib/block.c
        <SOURCE_DIR>/lib/codebook.c
        <SOURCE_DIR>/lib/envelope.c
        <SOURCE_DIR>/lib/floor0.c
        <SOURCE_DIR>/lib/floor1.c
        <SOURCE_DIR>/lib/info.c
        <SOURCE_DIR>/lib/lookup.c
        <SOURCE_DIR>/lib/lpc.c
        <SOURCE_DIR>/lib/lsp.c
        <SOURCE_DIR>/lib/mapping0.c
        <SOURCE_DIR>/lib/mdct.c
        <SOURCE_DIR>/lib/psy.c
        <SOURCE_DIR>/lib/registry.c
        <SOURCE_DIR>/lib/res0.c
        <SOURCE_DIR>/lib/sharedbook.c
        <SOURCE_DIR>/lib/smallft.c
        <SOURCE_DIR>/lib/synthesis.c
        <SOURCE_DIR>/lib/window.c
        -o ${VORBIS_INSTALL_DIR}/libvorbis.a
      BUILD_IN_SOURCE true
      BUILD_BYPRODUCTS ${_byproducts}
      INSTALL_COMMAND ""
      INSTALL_DIR ${VORBIS_INSTALL_DIR}
    )
  else()
    ExternalProject_Add(
      vorbis
      DOWNLOAD_COMMAND ""
      SOURCE_DIR ${CMAKE_THIRD_PARTY_DIR}/libvorbis-1.2.0
      CONFIGURE_COMMAND ""
      BUILD_COMMAND cl /c /EHsc /MT /I <SOURCE_DIR>/include /I ${OGG_INCLUDE_DIR}
        <SOURCE_DIR>/lib/analysis.c
        <SOURCE_DIR>/lib/bitrate.c
        <SOURCE_DIR>/lib/block.c
        <SOURCE_DIR>/lib/codebook.c
        <SOURCE_DIR>/lib/envelope.c
        <SOURCE_DIR>/lib/floor0.c
        <SOURCE_DIR>/lib/floor1.c
        <SOURCE_DIR>/lib/info.c
        <SOURCE_DIR>/lib/lookup.c
        <SOURCE_DIR>/lib/lpc.c
        <SOURCE_DIR>/lib/lsp.c
        <SOURCE_DIR>/lib/mapping0.c
        <SOURCE_DIR>/lib/mdct.c
        <SOURCE_DIR>/lib/psy.c
        <SOURCE_DIR>/lib/registry.c
        <SOURCE_DIR>/lib/res0.c
        <SOURCE_DIR>/lib/sharedbook.c
        <SOURCE_DIR>/lib/smallft.c
        <SOURCE_DIR>/lib/synthesis.c
        <SOURCE_DIR>/lib/vorbisfile.c
        <SOURCE_DIR>/lib/window.c
        && lib *.obj /OUT:${VORBIS_INSTALL_DIR}/libvorbis.lib
      BUILD_IN_SOURCE true
      BUILD_BYPRODUCTS ${_byproducts}
      INSTALL_COMMAND ""
      INSTALL_DIR ${VORBIS_INSTALL_DIR}
    )
  endif()
endif()


ExternalProject_Get_Property(
  vorbis
  source_dir
  install_dir
)

set(VORBIS_INCLUDE_DIR "${source_dir}/include")
if(NOT MINGW AND WIN32)
  set(VORBIS_LIBRARY_DIR "${install_dir}")
  set(VORBIS_LIBRARY "${install_dir}/libvorbis.lib")
  set(VORBIS_LIBRARIES "${install_dir}/libvorbis.lib" "${VORBIS_LIBRARIES}")
else()
  set(VORBIS_LIBRARY_DIR "${install_dir}")
  set(VORBIS_LIBRARY "${install_dir}/libvorbis.a")
  set(VORBIS_LIBRARIES "${install_dir}/libvorbis.a" "${VORBIS_LIBRARIES}")
endif()
if(NOT TARGET utils::vorbis)
  add_library(utils::vorbis INTERFACE IMPORTED)
  add_dependencies(utils::vorbis vorbis)
  set_target_properties(utils::vorbis PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${VORBIS_INCLUDE_DIR}")
  set_target_properties(utils::vorbis PROPERTIES INTERFACE_LINK_LIBRARIES "${VORBIS_LIBRARIES}")
endif()

mark_as_advanced( VORBIS_INCLUDE_DIR VORBIS_LIBRARIES VORBIS_LIBRARY VORBIS_LIBRARY_PATH )