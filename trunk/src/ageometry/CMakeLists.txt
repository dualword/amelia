SET(QT_USE_QTXML true)
SET(QT_USE_QTOPENGL true)

FIND_PACKAGE (Qt4 REQUIRED)
INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR})
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_SHARED)

IF(UNIX)
       MESSAGE(STATUS "System UNIX")
       FIND_PACKAGE(OpenGL REQUIRED)
       FIND_PACKAGE(X11)
       FIND_LIBRARY(XXF86VM_LIBRARY Xxf86vm)
       SET(SYSTEMSPECIFIC_LIBRARIES
             ${X11_LIBRARIES}
             ${XXF86VM_LIBRARY}
             ${OPENGL_LIBRARIES}
             jpeg
             png
       )
ENDIF(UNIX)

IF(APPLE)
	FIND_LIBRARY(IOKIT_LIBRARY IOKit REQUIRED)
	SET(SYSTEMSPECIFIC_LIBRARIES
		${SYSTEMSPECIFIC_LIBRARIES}
		${IOKIT_LIBRARY})
ENDIF(APPLE)

FIND_PACKAGE (Irrlicht REQUIRED)
INCLUDE_DIRECTORIES(${Irrlicht_INCLUDE_DIR})

SET(ageometry_SRCS
  QIrrWidget.cpp 
  QIrrWidgetRefresher.cpp 
  AGeometry.cpp 
  ATrack3DNode.cpp 
  ASTrack3DNode.cpp 
  AJet3DNode.cpp 
  AMisET3DNode.cpp 
  ALayerGUI.cpp 
  AViewport.cpp 
  AEventInfoScene.cpp 
  ASelectionInfoScene.cpp 
  CSceneNodeAnimatorCameraSphere.cpp
  CSceneNodeAnimatorCameraSwitch.cpp
  ATrackTableModel.cpp 
  AInterestingTrackTableModel.cpp 
  AComboTableModel.cpp 
  CRelativeScaleSceneNodeAnimator.cpp
  AAdvancedEventSettings.cpp
  AGeoPlugin.cpp
  ALineNode.cpp
  AGeometryHUD.cpp
  AFPSControl.cpp
  APoint3D.cpp

  QAbstractTableModelWithContextMenu.cpp
  QTableViewWithContextMenu.cpp
  )

SET(ageometry_H
  QIrrWidget.h 
  AGeometry.h 
  AGeometryHUD.h
  ALayerGUI.h 
  AViewport.h 
  AEventInfoScene.h 
  ASelectionInfoScene.h
  ATrackTableModel.h 
  AInterestingTrackTableModel.h 
  AComboTableModel.h
  AAdvancedEventSettings.h
  AGeoPlugin.h

  QAbstractTableModelWithContextMenu.h
  QTableViewWithContextMenu.h
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
  ${SYSTEMSPECIFIC_LIBRARIES}   
  Irrlicht
  amelia
  ABase
  AEventManager
  )
  


#INSTALL(TARGETS ageometry 
#  RUNTIME DESTINATION bin)
#INSTALL(FILES  media/AtlasGeometry.aml media/pack01.gui media/menu.aml
#  DESTINATION share/ageometry/media)
#INSTALL(FILES media/tours/karol.1.xml media/tours/michael.1.xml media/tours/test.xml
#  DESTINATION share/ageometry/tours)
#INSTALL(FILES media/events/special-higgs.xml
#  DESTINATION share/ageometry/events)

#INSTALL(FILES workspace/special/special-higgs.xml 
#  workspace/special/.metainfo
#  DESTINATION share/ageometry/workspace/special)

#INSTALL(FILES ageometry.desktop
#  DESTINATION share/applications)


CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)
