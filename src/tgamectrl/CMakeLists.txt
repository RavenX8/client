set(PROJECT TGameCtrl)
project(${PROJECT})

include_directories(
  SYSTEM ${SPDLOG_ROOT}
  SYSTEM ${DIRECTX_ROOT}/include
  SYSTEM ${CMAKE_THIRD_PARTY_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/include/tgamectrl
)

link_directories(
  ${DIRECTX_ROOT}/lib
  ${CMAKE_EXTERNAL_LIBRARY_DIR}
  ${CMAKE_THIRD_PARTY_LIBRARY_DIR}
)

add_linker_flags("/SAFESEH:NO")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTGAMECTRL_EXPORTS -D_WINDOWS -D_USRDLL -DWIN32")

set( SOURCES_HEADER
  include/tgamectrl/ActionEvent.h
  include/tgamectrl/ActionListenerList.h
  include/tgamectrl/CToolTip.h
  include/tgamectrl/IActionListener.h
  include/tgamectrl/IListCellRenderer.h
  include/tgamectrl/IScrollModel.h
  include/tgamectrl/ITControl.h
  include/tgamectrl/ITDraw.h
  include/tgamectrl/ITFont.h
  include/tgamectrl/ITreeModel.h
  include/tgamectrl/ITreeNode.h
  include/tgamectrl/ITreeNodeRenderer.h
  include/tgamectrl/ITSound.h
  include/tgamectrl/JComboBox.h
  include/tgamectrl/JContainer.h
  include/tgamectrl/JLetter.h
  include/tgamectrl/JListBox.h
  include/tgamectrl/JString.h
  include/tgamectrl/JStringParser.h
  include/tgamectrl/JTable.h
  include/tgamectrl/ResourceMgr.h
  include/tgamectrl/SinglelineString.h
  include/tgamectrl/SplitHangul.h
  include/tgamectrl/TabbedPane.h
  include/tgamectrl/TAnimation.h
  include/tgamectrl/TButton.h
  include/tgamectrl/TCandidateWindow.h
  include/tgamectrl/TCandidateWindowHanja.h
  include/tgamectrl/TCandidateWindowList.h
  include/tgamectrl/TCaption.h
  include/tgamectrl/TCheckBox.h
  include/tgamectrl/TComboBox.h
  include/tgamectrl/TCommand.h
  include/tgamectrl/TCommandQ.h
  include/tgamectrl/TContainer.h
  include/tgamectrl/TContainerItem.h
  include/tgamectrl/TControlMgr.h
  include/tgamectrl/TDefaultListCellRenderer.h
  include/tgamectrl/TDefaultTreeModel.h
  include/tgamectrl/TDefaultTreeNode.h
  include/tgamectrl/TDefaultTreeNodeRenderer.h
  include/tgamectrl/TDialog.h
  include/tgamectrl/TEditBox.h
  include/tgamectrl/TFrame.h
  include/tgamectrl/TGameCtrl.h
  include/tgamectrl/TGuage.h
  include/tgamectrl/TImage.h
  include/tgamectrl/TTIme.h
  include/tgamectrl/TListBox.h
  include/tgamectrl/TListCellRenderer.h
  include/tgamectrl/TMsgBox.h
  include/tgamectrl/TObject.h
  include/tgamectrl/TPane.h
  include/tgamectrl/TPushButton.h
  include/tgamectrl/TRadioBox.h
  include/tgamectrl/TRadioButton.h
  include/tgamectrl/Tree.h
  include/tgamectrl/TreePath.h
  include/tgamectrl/TScrollBar.h
  include/tgamectrl/TScrollBarType.h
  include/tgamectrl/TScrollBox.h
  include/tgamectrl/TScrollBoxType.h
  include/tgamectrl/TSplitString.h
  include/tgamectrl/TStatic.h
  include/tgamectrl/TStatusBar.h
  include/tgamectrl/WinCtrl.h
  include/tgamectrl/ZListBox.h
  res/resource.h
  src/stdafx.h
)

set( SOURCES_MAIN
  src/ActionEvent.cpp
  src/ActionListenerList.cpp
  src/CToolTip.cpp
  src/ITControl.cpp
  src/JComboBox.cpp
  src/JContainer.cpp
  src/JLetter.cpp
  src/JListBox.cpp
  src/JString.cpp
  src/JStringParser.cpp
  src/JTable.cpp
  src/ResourceMgr.cpp
  src/SinglelineString.cpp
  src/SplitHangul.cpp
  src/TabbedPane.cpp
  src/TAnimation.cpp
  src/TButton.cpp
  src/TCandidateWindow.cpp
  src/TCandidateWindowHanja.cpp
  src/TCandidateWindowList.cpp
  src/TCaption.cpp
  src/TCheckBox.cpp
  src/TComboBox.cpp
  src/TCommand.cpp
  src/TCommandQ.cpp
  src/TContainer.cpp
  src/TContainerItem.cpp
  src/TControlMgr.cpp
  src/TDefaultListCellRenderer.cpp
  src/TDefaultTreeModel.cpp
  src/TDefaultTreeNode.cpp
  src/TDefaultTreeNodeRenderer.cpp
  src/TDialog.cpp
  src/TEditBox.cpp
  src/TFrame.cpp
  src/TGameCtrl.cpp
  src/TGuage.cpp
  src/TImage.cpp
  src/TTIme.cpp
  src/TListBox.cpp
  src/TListCellRenderer.cpp
  src/TMsgBox.cpp
  src/TObject.cpp
  src/TPane.cpp
  src/TPushButton.cpp
  src/TRadioBox.cpp
  src/TRadioButton.cpp
  src/Tree.cpp
  src/TreePath.cpp
  src/TScrollBar.cpp
  src/TScrollBarType.cpp
  src/TScrollBox.cpp
  src/TScrollBoxType.cpp
  src/TSplitString.cpp
  src/TStatic.cpp
  src/TStatusBar.cpp
  src/WinCtrl.cpp
  src/ZListBox.cpp
)

set( RESOURCE_FILES
  res/TGameCtrl.rc
)

set_source_files_properties(${RESOURCE_FILES} PROPERTIES LANGUAGE RC)
add_library(${PROJECT} SHARED src/stdafx.cpp ${SOURCES_MAIN} ${SOURCES_HEADER} ${RESOURCE_FILES})
target_include_directories(${PROJECT} 
  PUBLIC 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_precompile_headers(${PROJECT} PRIVATE "src/stdafx.h")
set_target_properties(${PROJECT} PROPERTIES OUTPUT_NAME_DEBUG "TGameCtrl_d")
set_target_properties(${PROJECT} PROPERTIES OUTPUT_NAME_RELEASE "TGameCtrl_r")

target_link_libraries(${PROJECT} 
  PUBLIC 
    Imm32.lib winmm.lib
)


if (COMMAND cotire)
  cotire(${PROJECT})
endif()

