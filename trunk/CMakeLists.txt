CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(Amelia)

SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.BSD")
SET(CPACK_PACKAGE_CONTACT "AMELIA Development Team")
SET(CPACK_PACKAGE_NAME "amelia")
SET(CPACK_PACKAGE_VERSION "0.0.1")
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "0")
SET(CPACK_PACKAGE_VERSION_PATCH "1")
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt4-webkit (>= 4.4), libqt4-core (>=4.4), libqt4-gui (>=4.4)")
INCLUDE(CPack)

SET(QT_USE_QTXML true)
SET(QT_USE_QTOPENGL true)
SET(QT_USE_QTUITOOLS true)
SET(QT_USE_WEBKIT true)
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
SET(CMAKE_INCLUDE_CURRENT_DIR true)

FIND_PACKAGE (Qt4 REQUIRED)

IF(UNIX)
       MESSAGE(STATUS "System UNIX")
       FIND_PACKAGE(OpenGL REQUIRED)
       FIND_PACKAGE(X11)
       FIND_LIBRARY(XXF86VM_LIBRARY Xxf86vm)
       SET(SYSTEMSPECIFIC_LIBRARIES
             ${X11_LIBRARIES}
             ${XXF86VM_LIBRARY}
             ${OPENGL_LIBRARIES} 
       )
ENDIF(UNIX)

IF(${QT_VERSION_MINOR} LESS 4)
  MESSAGE(FATAL_ERROR "Need Qt 4.4.0 at least")
ENDIF(${QT_VERSION_MINOR} LESS 4)

INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR})

INCLUDE(${QT_USE_FILE})

FIND_PACKAGE (Irrlicht REQUIRED)

INCLUDE_DIRECTORIES(${Irrlicht_INCLUDE_DIR})

SET(amelia_SRCS
  src/QIrrWidget.cpp 
  src/ABase.cpp 
  src/AGeometry.cpp 
  src/ATrack3DNode.cpp 
  src/ATourBuilder.cpp 
  src/ATourManager.cpp 
  src/AXmlEvent.cpp 
  src/AUILoader.cpp 
  src/ALayerGUI.cpp 
  src/AViewport.cpp 
  src/AEventInfoScene.cpp 
  src/ASelectionInfoScene.cpp 
  src/CSceneNodeAnimatorCameraSphere.cpp
  src/CSceneNodeAnimatorCameraSwitch.cpp
  src/ATrackTableModel.cpp 
  src/AInterestingTrackTableModel.cpp 
  src/AComboTableModel.cpp 
  src/ATrackCombination.cpp 
  src/AMainView.cpp 
  src/AAnimationGUI.cpp 
  src/AEventManager.cpp
  src/AEventPackage.cpp
  src/CRelativeScaleSceneNodeAnimator.cpp
  src/AAdvancedEventSettings.cpp
  src/ATrack.cpp
  src/main.cpp
  src/QGraphicsClickablePixmapItem.cpp
  )

SET(amelia_H
  src/QIrrWidget.h 
  src/ABase.h 
  src/AGeometry.h 
  src/ATrack3DNode.h 
  src/ATourBuilder.h 
  src/ATourManager.h 
  src/AXmlEvent.h 
  src/AUILoader.h 
  src/ALayerGUI.h 
  src/AViewport.h 
  src/AEventInfoScene.h 
  src/ASelectionInfoScene.h
  src/ATrackTableModel.h 
  src/AInterestingTrackTableModel.h 
  src/AComboTableModel.h
  src/ATrackCombination.h 
  src/CSceneNodeAnimatorCameraSphere.h
  src/CSceneNodeAnimatorCameraSwitch.h
  src/AAnimationGUI.h 
  src/AEventManager.h
  src/AEventPackage.h
  src/CRelativeScaleSceneNodeAnimator.h
  src/AAdvancedEventSettings.h
  src/AMainView.h
  src/ATrack.h
  src/QGraphicsClickablePixmapItem.h
  )

SET(amelia_FILES
  ui/ui.qrc
  media/ageometry.qrc)


QT4_WRAP_CPP(amelia_SRCS ${amelia_H})

QT4_ADD_RESOURCES(amelia_SRCS ${amelia_FILES})

ADD_EXECUTABLE(amelia ${amelia_SRCS})

TARGET_LINK_LIBRARIES(amelia
  ${QT_LIBRARIES} 
  ${SYSTEMSPECIFIC_LIBRARIES}   
  Irrlicht    
  )
  


INSTALL(TARGETS amelia 
  RUNTIME DESTINATION bin)
INSTALL(FILES  media/AtlasGeometry.aml media/pack01.gui media/menu.aml
  DESTINATION share/amelia/media)
INSTALL(FILES media/tours/karol.1.xml media/tours/michael.1.xml media/tours/test.xml
  DESTINATION share/amelia/tours)
INSTALL(FILES media/events/special-higgs.xml
  DESTINATION share/amelia/events)

INSTALL(FILES workspace/special/special-higgs.xml 
  workspace/special/.metainfo
  DESTINATION share/amelia/workspace/special)

INSTALL(FILES amelia.desktop
  DESTINATION share/applications)


CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)
