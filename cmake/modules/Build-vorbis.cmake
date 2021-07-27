set(VORBIS_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})

set(_byproducts
  ${VORBIS_INSTALL_DIR}/lib/vorbis.lib
  ${VORBIS_INSTALL_DIR}/lib/vorbisenc.lib
  ${VORBIS_INSTALL_DIR}/lib/vorbisfile.lib
  ${VORBIS_INSTALL_DIR}/lib/libvorbis.a
)

#if(WIN32 OR MINGW)
    ExternalProject_Add(
      vorbis DEPENDS utils::ogg
      GIT_REPOSITORY https://gitlab.xiph.org/xiph/vorbis.git
      GIT_TAG 0657aee69dec8508a0011f47f3b69d7538e9d262
      GIT_SHALLOW true
      CMAKE_ARGS -G ${CMAKE_GENERATOR} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX=${VORBIS_INSTALL_DIR} -DOGG_LIBRARY=${OGG_LIBRARY} -DOGG_INCLUDE_DIR=${OGG_INCLUDE_DIR}
      BUILD_BYPRODUCTS ${_byproducts}
      INSTALL_DIR ${VORBIS_INSTALL_DIR}
    )
#endif()


ExternalProject_Get_Property(
  vorbis
  source_dir
  install_dir
)

set(VORBIS_INCLUDE_DIR "${install_dir}/include")
if(NOT MINGW AND WIN32)
  set(VORBIS_LIBRARY_DIR "${install_dir}")
  set(VORBIS_LIBRARY "${install_dir}/lib/vorbis.lib")
  set(VORBIS_LIBRARIES "${install_dir}/lib/vorbis.lib" "${install_dir}/lib/vorbisenc.lib" "${install_dir}/lib/vorbisfile.lib" "${VORBIS_LIBRARIES}")
else()
  set(VORBIS_LIBRARY_DIR "${install_dir}")
  set(VORBIS_LIBRARY "${install_dir}/lib/libvorbis.a")
  set(VORBIS_LIBRARIES "${install_dir}/lib/libvorbis.a" "${VORBIS_LIBRARIES}")
endif()
if(NOT TARGET utils::vorbis)
  add_library(utils::vorbis INTERFACE IMPORTED)
  add_dependencies(utils::vorbis vorbis)
  set_target_properties(utils::vorbis PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${VORBIS_INCLUDE_DIR}")
  set_target_properties(utils::vorbis PROPERTIES INTERFACE_LINK_LIBRARIES "${VORBIS_LIBRARIES}")
endif()

mark_as_advanced( VORBIS_INCLUDE_DIR VORBIS_LIBRARIES VORBIS_LIBRARY VORBIS_LIBRARY_PATH )