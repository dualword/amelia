SET(QT_USE_QTXML true)
SET(QT_USE_QTOPENGL true)

FIND_PACKAGE (Qt4 REQUIRED)
INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR})
INCLUDE(${QT_USE_FILE})


ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)

FIND_PACKAGE (Irrlicht REQUIRED)
INCLUDE_DIRECTORIES(${Irrlicht_INCLUDE_DIR})
LINK_DIRECTORIES(${Irrlicht_LIBRARY_DIR})

SET(atours_SRCS
  ATours.cpp
  ATour.cpp
  ATourBlock.cpp
  ATourManager.cpp
  ATourBuilder.cpp
  ATourPlayer.cpp
  ATourAction.cpp
  ATourCameraAction.cpp
  ATourMessageAction.cpp
  ATourLoadEventAction.cpp
  ATourPtCutAction.cpp
  ATourMonitorAction.cpp
  ATourMouseMoveAction.cpp
  ATourMouseConnectAction.cpp
  ATourViewportChangeAction.cpp
  ATourParticleFilterAction.cpp
  ATourSelectTrackAction.cpp
  ATourBookmarkAction.cpp
  )

SET(atours_H
  ATours.h
  ATourBlock.h
  ATourManager.h
  ATourBuilder.h
  ATourPlayer.h
  ATourAction.h
  ATourCameraAction.h
  ATourMessageAction.h
  ATourLoadEventAction.h
  ATourPtCutAction.h
  ATourMonitorAction.h
  ATourMouseMoveAction.h
  ATourMouseConnectAction.h
  ATourViewportChangeAction.h
  ATourParticleFilterAction.h
  ATourSelectTrackAction.h
  ATourBookmarkAction.h
  )

SET(atours_FILES
  atours.qrc
  media/skin.qrc)

QT4_WRAP_CPP(atours_SRCS ${atours_H})
QT4_ADD_RESOURCES(atours_SRCS ${atours_FILES})

ADD_LIBRARY(ATours SHARED ${atours_SRCS})
LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})
TARGET_LINK_LIBRARIES(ATours
  ${QT_LIBRARIES} 
  ${Irrlicht_LIBRARY} 
  amelia
  ABase
  AGeometry
  AEventManager
  Irrlicht
  )

INSTALL(TARGETS ATours
	DESTINATION lib/amelia/plugins)

INSTALL_FILES(tours test.xml)
INSTALL_FILES(events special-higgs.xml)

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)
