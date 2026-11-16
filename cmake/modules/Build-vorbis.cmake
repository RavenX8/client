set(VORBIS_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})

set(_vorbis_libs)
foreach(_l vorbis vorbisenc vorbisfile)
  list(APPEND _vorbis_libs "${VORBIS_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}${_l}${CMAKE_STATIC_LIBRARY_SUFFIX}")
endforeach()
set(_byproducts ${_vorbis_libs})

#if(WIN32 OR MINGW)
ExternalProject_Add(
  vorbis DEPENDS ogg
  GIT_REPOSITORY https://github.com/xiph/vorbis
  GIT_TAG 1b75110b5a2754ba1931d82dd83cb822b266a21d
  GIT_SHALLOW true
  CMAKE_ARGS -G ${CMAKE_GENERATOR} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX=${VORBIS_INSTALL_DIR} -DOGG_LIBRARY=${OGG_LIBRARY} -DOGG_INCLUDE_DIR=${OGG_INCLUDE_DIR} ${_ep_toolchain_args}
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
set(VORBIS_LIBRARY_DIR "${install_dir}/lib")
list(GET _vorbis_libs 0 VORBIS_LIBRARY)
# vorbisfile and vorbisenc depend on vorbis, which depends on ogg: keep that order for static linking
set(VORBIS_LIBRARIES "${install_dir}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}vorbisfile${CMAKE_STATIC_LIBRARY_SUFFIX}" "${install_dir}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}vorbisenc${CMAKE_STATIC_LIBRARY_SUFFIX}" "${VORBIS_LIBRARY}" ${OGG_LIBRARIES})
if(NOT TARGET utils::vorbis)
  add_library(utils::vorbis INTERFACE IMPORTED)
  add_dependencies(utils::vorbis vorbis)
  set_target_properties(utils::vorbis PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${VORBIS_INCLUDE_DIR}")
  set_target_properties(utils::vorbis PROPERTIES INTERFACE_LINK_LIBRARIES "${VORBIS_LIBRARIES}")
endif()

mark_as_advanced( VORBIS_INCLUDE_DIR VORBIS_LIBRARIES VORBIS_LIBRARY VORBIS_LIBRARY_PATH )