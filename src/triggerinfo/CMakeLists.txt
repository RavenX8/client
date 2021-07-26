set(PROJECT TriggerInfo)
project(${PROJECT})

include_directories(
  SYSTEM ${SPDLOG_ROOT}
  SYSTEM ${DIRECTX_ROOT}/include
  SYSTEM ${CMAKE_THIRD_PARTY_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/include/triggerinfo
)

if (64BIT)
  link_directories(${DIRECTX_ROOT}/lib/x64)
else()
  link_directories(${DIRECTX_ROOT}/lib)
endif()

link_directories(
  ${CMAKE_THIRD_PARTY_LIBRARY_DIR}
)

add_linker_flags("/SAFESEH:NO")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTRIGGERINFO_EXPORTS -D_WINDOWS -D_USRDLL -DWIN32")

set( SOURCES_HEADER
  include/triggerinfo/reg.h
  include/triggerinfo/AdapterInfo.h
  include/triggerinfo/CpuInfo.h
  include/triggerinfo/dispinfo.h
  include/triggerinfo/Http.h
  include/triggerinfo/SysInfo.h
  include/triggerinfo/SystemInfo.h
  include/triggerinfo/TriggerInfo.h
  include/triggerinfo/__d3d9.h
  src/getdxver.h
  src/stdafx.h
)

set( SOURCES_MAIN
  src/reg.cpp
  src/AdapterInfo.cpp
  src/CpuInfo.cpp
  src/getdxver.cpp
  src/Http.cpp
  src/SysInfo.cpp
  src/TriggerInfo.cpp
  src/__d3d9.cpp
)

add_library(${PROJECT} SHARED src/stdafx.cpp ${SOURCES_MAIN} ${SOURCES_HEADER})
target_include_directories(${PROJECT} 
  PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
set_target_properties(${PROJECT} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "src/stdafx.h")
set_target_properties(${PROJECT} PROPERTIES OUTPUT_NAME_DEBUG "TriggerInfo_d")

target_link_libraries(${PROJECT} wininet.lib d3d9.lib dxguid.lib Version.lib)

if (COMMAND cotire)
  cotire(${PROJECT})
endif()