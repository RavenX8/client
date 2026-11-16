set(OGG_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})

# Library file name for the active toolchain: ogg.lib (MSVC/clang-cl), libogg.a (gcc/clang)
set(OGG_LIBRARY "${OGG_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}ogg${CMAKE_STATIC_LIBRARY_SUFFIX}")
set(_byproducts ${OGG_LIBRARY})

# Forward the toolchain so the sub-build targets the same platform (cross builds)
set(_ep_toolchain_args)
if(CMAKE_TOOLCHAIN_FILE)
  list(APPEND _ep_toolchain_args -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE})
endif()

#if(WIN32 OR MINGW)
  ExternalProject_Add(
    ogg
    GIT_REPOSITORY https://gitlab.xiph.org/xiph/ogg.git
    GIT_TAG v1.3.6
    GIT_SHALLOW true
    CMAKE_ARGS -G ${CMAKE_GENERATOR} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX=${OGG_INSTALL_DIR} ${_ep_toolchain_args}
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
set(OGG_LIBRARY_DIR "${install_dir}/lib")
set(OGG_LIBRARIES "${OGG_LIBRARY}")
if(NOT TARGET utils::ogg)
  add_library(utils::ogg INTERFACE IMPORTED)
  add_dependencies(utils::ogg ogg)
  set_target_properties(utils::ogg PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${OGG_INCLUDE_DIR}")
  set_target_properties(utils::ogg PROPERTIES INTERFACE_LINK_LIBRARIES "${OGG_LIBRARIES}")
endif()

mark_as_advanced( OGG_INCLUDE_DIR OGG_LIBRARIES OGG_LIBRARY OGG_LIBRARY_PATH )