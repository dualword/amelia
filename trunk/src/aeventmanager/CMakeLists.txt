SET(QT_USE_QTXML 1)

FIND_PACKAGE (Qt4 REQUIRED)
INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR})
INCLUDE(${QT_USE_FILE})


ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)

SET(aeventmanager_SRCS
  AEventManager.cpp
  AEventManagerScene.cpp
  AEventManagerTreeView.cpp
  AEventPackage.cpp
  ATrack.cpp
  ASTrack.cpp
  AJet.cpp
  AMisET.cpp
  AShower.cpp
  AFCALShower.cpp

  AXmlEvent.cpp
  AEvent.cpp
  AFilteredEvent.cpp

  
  ATrackFilter.cpp
  APtFilter.cpp
  AModelFilter.cpp
  AParticleFilter.cpp
)

SET(aeventmanager_H
  AEventManager.h
  AEventManagerScene.h
  AEventManagerTreeView.h
  AEventPackage.h
  ATrack.h
  ASTrack.h

  AXmlEvent.h
  AEvent.h
  AFilteredEvent.h

  ATrackFilter.h
  APtFilter.h
  AModelFilter.h
  AParticleFilter.h
)

QT4_WRAP_CPP(aeventmanager_SRCS ${aeventmanager_H})

ADD_LIBRARY(AEventManager SHARED ${aeventmanager_SRCS})
LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})
TARGET_LINK_LIBRARIES(AEventManager
  ${QT_LIBRARIES} 
  amelia)

ADD_SUBDIRECTORY(tests)