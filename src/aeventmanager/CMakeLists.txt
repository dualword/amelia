SET(QT_USE_QTXML 1)

FIND_PACKAGE (Qt4 REQUIRED)
FIND_PACKAGE (Irrlicht REQUIRED)
INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${Irrlicht_INCLUDE_DIR})
LINK_DIRECTORIES(${Irrlicht_LIBRARY_DIR})

INCLUDE(${QT_USE_FILE})


ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_SHARED)

SET(aeventmanager_SRCS
  AEventManager.cpp
  AEventManagerScene.cpp
  AEventManagerTreeView.cpp
  AEventPackage.cpp

  AEventAnalysisData.cpp
  AEventObjectCollection.cpp

  AEventObject.cpp
  ATrack.cpp
  ASTrack.cpp
  ARTrack.cpp
  AJet.cpp
  AMisET.cpp
  AShower.cpp
  AFCALShower.cpp
  ATrackCombination.cpp

  AXmlEvent.cpp
  AEvent.cpp
  AFilteredEvent.cpp

  AEventObjectFilter.cpp
  APtFilter.cpp
  AModelFilter.cpp
  AParticleFilter.cpp
  ACalorimeterFilter.cpp
)

SET(aeventmanager_H
  AEventManager.h
  AEventManagerScene.h
  AEventManagerTreeView.h
  AEventPackage.h

  AEventAnalysisData.h
  AEventObjectCollection.h

  AXmlEvent.h
  AEvent.h
  AFilteredEvent.h

  AEventObject.h
  ATrack.h
  AJet.h
  AShower.h
  ATrackCombination.h

  AEventObjectFilter.h
  APtFilter.h
  AModelFilter.h
  AParticleFilter.h
  ACalorimeterFilter.h
)

QT4_WRAP_CPP(aeventmanager_SRCS ${aeventmanager_H})

ADD_LIBRARY(AEventManager SHARED ${aeventmanager_SRCS})
LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})
TARGET_LINK_LIBRARIES(AEventManager
  ${QT_LIBRARIES}
  ${Irrlicht_LIBRARY}
  amelia)

INSTALL(TARGETS AEventManager
  DESTINATION lib/amelia/plugins)

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)
