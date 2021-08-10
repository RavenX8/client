set(OGG_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})

set(_byproducts
  ${OGG_INSTALL_DIR}/lib/ogg.lib
  ${OGG_INSTALL_DIR}/lib/ogg.a
)

#if(WIN32 OR MINGW)
  ExternalProject_Add(
    ogg
    GIT_REPOSITORY https://gitlab.xiph.org/xiph/ogg.git
    GIT_TAG e1774cd77f471443541596e09078e78fdc342e4f
    GIT_SHALLOW true
    CMAKE_ARGS -G ${CMAKE_GENERATOR} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX=${OGG_INSTALL_DIR}
    BUILD_BYPRODUCTS ${_byproducts}
    INSTALL_DIR ${OGG_INSTALL_DIR}
  )
#endif()


ExternalProject_Get_Property(
  ogg
  source_dir
  install_dir
)

set(OGG_INCLUDE_DIR "${install_dir}/include")
if(NOT MINGW AND WIN32)
  set(OGG_LIBRARY_DIR "${install_dir}")
  set(OGG_LIBRARY "${install_dir}/lib/ogg.lib")
  set(OGG_LIBRARIES "${install_dir}/lib/ogg.lib" "${OGG_LIBRARIES}")
else()
  set(OGG_LIBRARY_DIR "${install_dir}")
  set(OGG_LIBRARY "${install_dir}/lib/ogg.a")
  set(OGG_LIBRARIES "${install_dir}/lib/ogg.a" "${OGG_LIBRARIES}")
endif()
if(NOT TARGET utils::ogg)
  add_library(utils::ogg INTERFACE IMPORTED)
  add_dependencies(utils::ogg ogg)
  set_target_properties(utils::ogg PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${OGG_INCLUDE_DIR}")
  set_target_properties(utils::ogg PROPERTIES INTERFACE_LINK_LIBRARIES "${OGG_LIBRARIES}")
endif()

mark_as_advanced( OGG_INCLUDE_DIR OGG_LIBRARIES OGG_LIBRARY OGG_LIBRARY_PATH )