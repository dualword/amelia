SET(QT_USE_PHONON 1)
SET(QT_USE_QTXML 1)

FIND_PACKAGE (Qt4 REQUIRED)
FIND_PACKAGE (Irrlicht REQUIRED)
INCLUDE_DIRECTORIES(${Irrlicht_INCLUDE_DIR})

IF(QT_PHONON_FOUND)
  SET(PHONON_FOUND true)
ELSE(QT_PHONON_FOUND)
  FIND_PACKAGE(KDE4)
  INCLUDE(KDE4Defaults)
  FIND_PACKAGE(Phonon)
  #For some reason, KDE4 cmake rests the LIBRARY_OUTPUT_PATH to libs..
  SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/plugins)
ENDIF(QT_PHONON_FOUND)

IF(PHONON_FOUND)
  INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR})
  INCLUDE_DIRECTORIES(${PHONON_INCLUDES})
  INCLUDE(${QT_USE_FILE})



  ADD_DEFINITIONS(${QT_DEFINITIONS})
  ADD_DEFINITIONS(-DQT_PLUGIN)
  ADD_DEFINITIONS(-DQT_SHARED)
  
  SET(acontextmedia_SRCS
    AContextMedia.cpp
    ACustomPlayer.cpp)
  
  SET(acontextmedia_H
    AContextMedia.h)
  
  SET(acontextmedia_FILES
    acontextmedia.qrc
    media/icons.qrc)
  
  QT4_WRAP_CPP(acontextmedia_SRCS ${acontextmedia_H})
  QT4_ADD_RESOURCES(acontextmedia_SRCS ${acontextmedia_FILES})
  
  ADD_LIBRARY(AContextMedia SHARED ${acontextmedia_SRCS})
  LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})
  TARGET_LINK_LIBRARIES(AContextMedia
    ${QT_LIBRARIES} 
    ${PHONON_LIBS}
    AGeometry
    amelia)

  INSTALL(TARGETS AContextMedia
    DESTINATION lib/amelia/plugins)
    
ENDIF(PHONON_FOUND)
