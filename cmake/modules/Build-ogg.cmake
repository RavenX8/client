set(OGG_INSTALL_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

set(_byproducts
  ${OGG_INSTALL_DIR}/libogg.lib
  ${OGG_INSTALL_DIR}/libogg.a
)

if(WIN32 OR MINGW)
  if(MINGW)
    ExternalProject_Add(
      ogg
      DOWNLOAD_COMMAND ""
      SOURCE_DIR ${CMAKE_SUBMODULE_DIR}/libogg-1.1.3
      CONFIGURE_COMMAND ""
      BUILD_COMMAND gcc -shared <SOURCE_DIR>/src/bitwise.c
      <SOURCE_DIR>/src/framing.c -o ${OGG_INSTALL_DIR}/libogg.a
      BUILD_IN_SOURCE true
      BUILD_BYPRODUCTS ${_byproducts}
      INSTALL_COMMAND ""
      INSTALL_DIR ${OGG_INSTALL_DIR}
    )
  else()
    ExternalProject_Add(
      ogg
      DOWNLOAD_COMMAND ""
      SOURCE_DIR ${CMAKE_SUBMODULE_DIR}/libogg-1.1.3
      CONFIGURE_COMMAND ""
      BUILD_COMMAND cl /c /EHsc /MT /I <SOURCE_DIR>/include
      <SOURCE_DIR>/src/bitwise.c
      <SOURCE_DIR>/src/framing.c
      && lib *.obj /OUT:${OGG_INSTALL_DIR}/libogg.lib
      BUILD_IN_SOURCE true
      BUILD_BYPRODUCTS ${_byproducts}
      INSTALL_COMMAND ""
      INSTALL_DIR ${OGG_INSTALL_DIR}
    )
  endif()
endif()


ExternalProject_Get_Property(
  ogg
  source_dir
  install_dir
)

set(OGG_INCLUDE_DIR "${source_dir}/include")
if(NOT MINGW AND WIN32)
  set(OGG_LIBRARY_DIR "${install_dir}")
  set(OGG_LIBRARY "${install_dir}/libogg.lib")
  set(OGG_LIBRARIES "${install_dir}/libogg.lib" "${OGG_LIBRARIES}")
else()
  set(OGG_LIBRARY_DIR "${install_dir}")
  set(OGG_LIBRARY "${install_dir}/libogg.a")
  set(OGG_LIBRARIES "${install_dir}/libogg.a" "${OGG_LIBRARIES}")
endif()
if(NOT TARGET utils::ogg)
  add_library(utils::ogg INTERFACE IMPORTED)
  add_dependencies(utils::ogg ogg)
  set_target_properties(utils::ogg PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${OGG_INCLUDE_DIR}")
  set_target_properties(utils::ogg PROPERTIES INTERFACE_LINK_LIBRARIES "${OGG_LIBRARIES}")
endif()

mark_as_advanced( OGG_INCLUDE_DIR OGG_LIBRARIES OGG_LIBRARY OGG_LIBRARY_PATH )