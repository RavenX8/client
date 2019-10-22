set(SQLITE3_INSTALL_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

set(_byproducts
  ${SQLITE3_INSTALL_DIR}/libsqlite3.lib
  ${SQLITE3_INSTALL_DIR}/libsqlite3.so
  ${SQLITE3_INSTALL_DIR}/libsqlite3.a
)

if(WIN32 OR MINGW)
  if(MINGW)
    ExternalProject_Add(
      sqlite3
      DOWNLOAD_COMMAND ""
      SOURCE_DIR ${CMAKE_SUBMODULE_DIR}/sqlite3
      CONFIGURE_COMMAND ""
      BUILD_COMMAND gcc -shared sqlite3.c -o ${SQLITE3_INSTALL_DIR}/libsqlite3.a
      BUILD_IN_SOURCE true
      BUILD_BYPRODUCTS ${_byproducts}
      INSTALL_COMMAND ""
      INSTALL_DIR ${SQLITE3_INSTALL_DIR}
    )
  else()
    ExternalProject_Add(
      sqlite3
      DOWNLOAD_COMMAND ""
      SOURCE_DIR ${CMAKE_SUBMODULE_DIR}/sqlite3
      CONFIGURE_COMMAND ""
      BUILD_COMMAND cl /c /EHsc /MT sqlite3.c && lib sqlite3.obj /OUT:${SQLITE3_INSTALL_DIR}/libsqlite3.lib
      BUILD_IN_SOURCE true
      BUILD_BYPRODUCTS ${_byproducts}
      INSTALL_COMMAND ""
      INSTALL_DIR ${SQLITE3_INSTALL_DIR}
    )
  endif()
endif()


ExternalProject_Get_Property(
  sqlite3
  source_dir
  install_dir
)

set(SQLITE3_INCLUDE_DIR "${source_dir}")
if(NOT MINGW AND WIN32)
  set(SQLITE3_LIBRARY_DIR "${install_dir}")
  set(SQLITE3_LIBRARY "${install_dir}/libsqlite3.lib")
  set(SQLITE3_LIBRARIES "${install_dir}/libsqlite3.lib" "${SQLITE3_LIBRARIES}")
else()
  set(SQLITE3_LIBRARY_DIR "${install_dir}")
  set(SQLITE3_LIBRARY "${install_dir}/libsqlite3.a")
  set(SQLITE3_LIBRARIES "${install_dir}/libsqlite3.a" "${SQLITE3_LIBRARIES}")
endif()
if(NOT TARGET sqlite3::sqlite3)
  add_library(sqlite3::sqlite3 INTERFACE IMPORTED)
  add_dependencies(sqlite3::sqlite3 sqlite3)
  set_target_properties(sqlite3::sqlite3 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${SQLITE3_INCLUDE_DIR}")
  set_target_properties(sqlite3::sqlite3 PROPERTIES INTERFACE_LINK_LIBRARIES "${SQLITE3_LIBRARIES}")
endif()

mark_as_advanced( SQLITE3_INCLUDE_DIR SQLITE3_LIBRARIES SQLITE3_LIBRARY SQLITE3_LIBRARY_PATH )