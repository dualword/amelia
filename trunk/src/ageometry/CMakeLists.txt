SET(QT_USE_QTXML true)
SET(QT_USE_QTOPENGL true)

FIND_PACKAGE (Qt4 REQUIRED)
INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR})
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_SHARED)

FIND_PACKAGE (Irrlicht REQUIRED)
INCLUDE_DIRECTORIES(${Irrlicht_INCLUDE_DIR})
LINK_DIRECTORIES(${Irrlicht_LIBRARY_DIR})

SET(ageometry_SRCS
  QIrrWidget.cpp 
  QIrrWidgetRefresher.cpp 
  AGeometry.cpp 
  AEventObject3DNode.cpp 
  AHelix3DNode.cpp 
  ASTrack3DNode.cpp 
  ARTrack3DNode.cpp 
  AJet3DNode.cpp 
  AMisET3DNode.cpp 
  ACaloTower3DNode.cpp 
  ALayerGUI.cpp 
  AViewport.cpp 
  AEventInfoScene.cpp 
  ASelectionInfoScene.cpp 
  CSceneNodeAnimatorCameraSphere.cpp
  CSceneNodeAnimatorCameraFOV.cpp
  CSceneNodeAnimatorCameraSwitch.cpp
  ATrackTreeModel.cpp 
  AInterestingTrackTableModel.cpp 
  CRelativeScaleSceneNodeAnimator.cpp
  AAdvancedEventSettings.cpp
  AGeoPlugin.cpp
  ALineNode.cpp
  AGeometryHUD.cpp
  ACameraControl.cpp
  ASphereControl.cpp
  AFPSControl.cpp
  AFOVControl.cpp
  APoint3D.cpp
  ASnapshotTool.cpp

  QAbstractTableModelWithContextMenu.cpp
  QAbstractItemModelWithContextMenu.cpp
  QTreeViewWithContextMenu.cpp
  QTableViewWithContextMenu.cpp
  QAbstractTreeItem.cpp
  )

SET(ageometry_H
  QIrrWidget.h 
  AGeometry.h 
  AEventObject3DNode.h
  AHelix3DNode.h
  ASTrack3DNode.h
  ARTrack3DNode.h 
  AJet3DNode.h 
  AMisET3DNode.h
  ACaloTower3DNode.h
  AGeometryHUD.h
  ALayerGUI.h 
  AViewport.h 
  AEventInfoScene.h 
  ASelectionInfoScene.h
  ATrackTreeModel.h 
  AInterestingTrackTableModel.h 
  AAdvancedEventSettings.h
  AGeoPlugin.h
  ASnapshotTool.h
  ACameraControl.h

  QAbstractTableModelWithContextMenu.h
  QAbstractItemModelWithContextMenu.h
  QAbstractTreeItem.h
  )

SET(ageometry_FILES
  media/geoskin.qrc
  ageometry.qrc)

SET(ageometry_UI
	geometry.ui
	detectorvisibility.ui
	eventadvanced.ui)

QT4_WRAP_UI(ageometry_SRCS ${ageometry_UI})
QT4_WRAP_CPP(ageometry_SRCS ${ageometry_H})
QT4_ADD_RESOURCES(ageometry_SRCS ${ageometry_FILES})

ADD_LIBRARY(AGeometry SHARED ${ageometry_SRCS})
SET_PROPERTY(TARGET AGeometry PROPERTY LINK_INTERFACE_LIBRARIES "")
TARGET_LINK_LIBRARIES(AGeometry
  ${QT_LIBRARIES} 
  ${Irrlicht_LIBRARY}
  amelia
  ABase
  AEventManager
  )
  
INSTALL(TARGETS AGeometry
  DESTINATION lib/amelia/plugins)

INSTALL_FILES(media AtlasGeometry.aml )
INSTALL_FILES(media pack01.gui )
INSTALL_FILES(media logo.aml )
