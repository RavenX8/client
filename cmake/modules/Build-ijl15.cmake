set(IJL15_INSTALL_DIR ${CMAKE_SUBMODULE_DIR}/ijl-1.5)

set(_byproducts
  ${IJL15_INSTALL_DIR}/IJL15.lib
)

if(WIN32)
  set(IJL15_LIBRARY "${IJL15_INSTALL_DIR}/IJL15.lib")
  set(IJL15_LIBRARIES "${IJL15_LIBRARY}")
endif()

set(IJL15_INCLUDE_DIR "${IJL15_INSTALL_DIR}")

if(NOT TARGET utils::ijl15)
  add_library(utils::ijl15 INTERFACE IMPORTED)
  add_dependencies(utils::ijl15 ijl15)
  set_target_properties(utils::ijl15 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${IJL15_INCLUDE_DIR}")
  set_target_properties(utils::ijl15 PROPERTIES INTERFACE_LINK_LIBRARIES "${IJL15_LIBRARIES}")
endif()

mark_as_advanced(IJL15_INCLUDE_DIR IJL15_LIBRARY IJL15_LIBRARIES)
