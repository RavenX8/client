set(EXPAT_INSTALL_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

set(_byproducts
  ${EXPAT_INSTALL_DIR}/xmlparse.lib
  ${EXPAT_INSTALL_DIR}/xmlparse.a
)

if(WIN32 OR MINGW)
  if(MINGW)
    ExternalProject_Add(
      expat
      DOWNLOAD_COMMAND ""
      SOURCE_DIR ${CMAKE_SUBMODULE_DIR}/expat-2.0.1
      CONFIGURE_COMMAND ""
      BUILD_COMMAND gcc -shared <SOURCE_DIR>/lib/xmlparse.c
        <SOURCE_DIR>/lib/xmlrole.c
        <SOURCE_DIR>/lib/xmltok.c
        <SOURCE_DIR>/lib/xmltok_impl.c
        <SOURCE_DIR>/lib/xmltok_ns.c -o ${EXPAT_INSTALL_DIR}/xmlparse.a
      BUILD_IN_SOURCE true
      BUILD_BYPRODUCTS ${_byproducts}
      INSTALL_COMMAND ""
      INSTALL_DIR ${EXPAT_INSTALL_DIR}
    )
  else()
    ExternalProject_Add(
      expat
      DOWNLOAD_COMMAND ""
      SOURCE_DIR ${CMAKE_SUBMODULE_DIR}/expat-2.0.1
      CONFIGURE_COMMAND ""
      BUILD_COMMAND cl /LD /EHsc /MT /D "COMPILED_FROM_DSP" /D "_WINDOWS" /D "_USRDLL" /D "EXPAT_EXPORTS" /D "_MBCS" 
        <SOURCE_DIR>/lib/xmlparse.c
        <SOURCE_DIR>/lib/xmlrole.c
        <SOURCE_DIR>/lib/xmltok.c
        <SOURCE_DIR>/lib/xmltok_impl.c
        <SOURCE_DIR>/lib/xmltok_ns.c
        /link /def:<SOURCE_DIR>/lib/libexpat.def
        /out:${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/xmlparse.dll
        /implib:${EXPAT_INSTALL_DIR}/xmlparse.lib
      BUILD_IN_SOURCE true
      BUILD_BYPRODUCTS ${_byproducts}
      INSTALL_COMMAND ""
      INSTALL_DIR ${EXPAT_INSTALL_DIR}
    )
  endif()
endif()


ExternalProject_Get_Property(
  expat
  source_dir
  install_dir
)

set(EXPAT_INCLUDE_DIR "${source_dir}/lib")
if(NOT MINGW AND WIN32)
  set(EXPAT_LIBRARY_DIR "${install_dir}")
  set(EXPAT_LIBRARY "${install_dir}/xmlparse.lib")
  set(EXPAT_LIBRARIES "${install_dir}/xmlparse.lib" "${EXPAT_LIBRARIES}")
else()
  set(EXPAT_LIBRARY_DIR "${install_dir}")
  set(EXPAT_LIBRARY "${install_dir}/xmlparse.a")
  set(EXPAT_LIBRARIES "${install_dir}/xmlparse.a" "${EXPAT_LIBRARIES}")
endif()
if(NOT TARGET utils::expat)
  add_library(utils::expat INTERFACE IMPORTED)
  add_dependencies(utils::expat expat)
  set_target_properties(utils::expat PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${EXPAT_INCLUDE_DIR}")
  set_target_properties(utils::expat PROPERTIES INTERFACE_LINK_LIBRARIES "${EXPAT_LIBRARIES}")
endif()

mark_as_advanced( EXPAT_INCLUDE_DIR EXPAT_LIBRARIES EXPAT_LIBRARY EXPAT_LIBRARY_PATH )