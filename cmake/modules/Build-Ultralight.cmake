set(ULTRALIGHT_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})


if(WIN32 OR MINGW)
  set(DOWNLOAD_URL https://ultralight-sdk.sfo2.cdn.digitaloceanspaces.com/ultralight-sdk-latest-win-x86.7z)
  set(EXEC_POSTFIX .dll)
  set(_byproducts
    ${ULTRALIGHT_INSTALL_DIR}/lib/Ultralight.lib
    ${ULTRALIGHT_INSTALL_DIR}/lib/UltralightCore.lib
  )
else()
  set(DOWNLOAD_URL https://ultralight-sdk.sfo2.cdn.digitaloceanspaces.com/ultralight-sdk-latest-linux-x64.7z)
  set(EXEC_POSTFIX )
  set(_byproducts
    ${ULTRALIGHT_INSTALL_DIR}/bin/Ultralight.so
    ${ULTRALIGHT_INSTALL_DIR}/bin/UltralightCore.so
  )
endif()

ExternalProject_Add(
  ultralight
  URL ${DOWNLOAD_URL}
  UPDATE_DISCONNECTED true
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ${COPY_SCRIPT_PATH} <SOURCE_DIR>/bin <INSTALL_DIR>/bin "*"
  COMMAND ${COPY_SCRIPT_PATH} <SOURCE_DIR>/include <INSTALL_DIR>/include "*"
  COMMAND ${COPY_SCRIPT_PATH} <SOURCE_DIR>/lib <INSTALL_DIR>/lib "*"
  INSTALL_COMMAND ""
  BUILD_BYPRODUCTS ${_byproducts}
  INSTALL_DIR ${ULTRALIGHT_INSTALL_DIR}
)
ExternalProject_Get_Property(
  ultralight
  install_dir
)

if(WIN32)
  set(ULTRALIGHT_LIBRARIES "${install_dir}/lib/Ultralight.lib" "${install_dir}/lib/UltralightCore.lib" "${install_dir}/lib/WebCore.lib")
  set(ULTRALIGHT_INSTALL_LIBS "${install_dir}/bin/Ultralight.dll" "${install_dir}/bin/UltralightCore.dll" "${install_dir}/bin/WebCore.bin")
else()
  set(ULTRALIGHT_LIBRARIES "${install_dir}/bin/Ultralight.so" "${install_dir}/bin/UltralightCore.so" "${install_dir}/bin/WebCore.so")
  set(ULTRALIGHT_INSTALL_LIBS "${install_dir}/bin/Ultralight.so" "${install_dir}/bin/UltralightCore.so" "${install_dir}/bin/WebCore.so")
endif()

set(ULTRALIGHT_INCLUDE_DIR "${source_dir}/include")

if(NOT TARGET ultralight::ultralight)
  add_library(ultralight::ultralight INTERFACE IMPORTED)
  add_dependencies(ultralight::ultralight ultralight)
  set_target_properties(ultralight::ultralight PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${ULTRALIGHT_INCLUDE_DIR}")
  set_target_properties(ultralight::ultralight PROPERTIES INTERFACE_LINK_LIBRARIES "${ULTRALIGHT_LIBRARIES}")
endif()

mark_as_advanced( ULTRALIGHT_INCLUDE_DIR )