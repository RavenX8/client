set(PROJECT TriggerVFS)
project(${PROJECT})

include_directories(
  SYSTEM ${SPDLOG_ROOT}
  SYSTEM ${EXTERNAL_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/include/triggervfs
)

link_directories(
  ${EXTERNAL_LIBRARY_DIR}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__DLL -DTRIGGERVFS_EXPORTS")

set( SOURCES_HEADER
  include/triggervfs/BlankInfo.h
  include/triggervfs/classCRC.h
  include/triggervfs/crc.h
  include/triggervfs/DllMain.h
  include/triggervfs/ErrorEtc.h
  include/triggervfs/GlobalData.h
  include/triggervfs/libCrc.h
  include/triggervfs/libFile.h
  include/triggervfs/libString.h
  include/triggervfs/Macro.h
  include/triggervfs/TriggerVFS.h
  include/triggervfs/VFS.h
  include/triggervfs/VFS_Manager.h
  include/triggervfs/__File.h
  res/resource.h
)

set( SOURCES_MAIN
  src/BlankInfo.cpp
  src/classCRC.cpp
  src/DllMain.cpp
  src/ErrorEtc.cpp
  src/GlobalData.cpp
  src/libCrc.cpp
  src/libFile.cpp
  src/libString.cpp
  src/stdafx.cpp
  src/stdafx.h
  src/TriggerVFS.cpp
  src/VFS.cpp
  src/VFS_Manager.cpp
)

set( RESOURCE_FILES
  res/TGameCtrl.rc
)

set_source_files_properties(${RESOURCE_FILES} PROPERTIES LANGUAGE RC)
add_library(${PROJECT} SHARED ${SOURCES_MAIN} ${SOURCES_HEADER})
target_include_directories(${PROJECT} 
  PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
set_target_properties(${PROJECT} PROPERTIES OUTPUT_NAME_DEBUG "TriggerVFS_d")

