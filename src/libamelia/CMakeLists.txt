SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

SET(QT_USE_QTOPENGL true)
SET(QT_USE_QTXML true)

FIND_PACKAGE (Qt4 REQUIRED)
INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR})
INCLUDE(${QT_USE_FILE})


ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)

SET(amelia_SRCS
	QGraphicsClickablePixmapItem.cpp
	QGraphicsClickableSimpleTextItem.cpp
	QGraphicsItemLayout.cpp
	AAnimationGUI.cpp
	AMainView.cpp
	APlugin.cpp
	AMELIA.cpp
  )

SET(amelia_H
	QGraphicsClickablePixmapItem.h
	QGraphicsClickableSimpleTextItem.h
	AAnimationGUI.h
	AMainView.h
 	APlugin.h
	AMELIA.h
    )

SET(amelia_FILES)

QT4_WRAP_CPP(amelia_SRCS ${amelia_H})
QT4_ADD_RESOURCES(amelia_SRCS ${amelia_FILES})

ADD_LIBRARY(amelia SHARED ${amelia_SRCS})
TARGET_LINK_LIBRARIES(amelia
  ${QT_LIBRARIES})
