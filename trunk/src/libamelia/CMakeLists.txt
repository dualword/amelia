SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

SET(QT_USE_QTOPENGL true)
SET(QT_USE_QTXML true)

FIND_PACKAGE (Qt4 REQUIRED)
INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR})
INCLUDE(${QT_USE_FILE})


ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_SHARED)

SET(amelia_SRCS
	QGraphicsClickablePixmapItem.cpp
	QGraphicsClickableSimpleTextItem.cpp
	QGraphicsItemLayout.cpp
	QGraphicsItemTransformAnimation.cpp
	QTimeLineProxy.cpp
	QBetterMimeData.cpp
	QBoolSync.cpp
	QDoubleSync.cpp
	AAnimationGUI.cpp
	AAnimationLayoutGUI.cpp
	AHideAButton.cpp
	AMainView.cpp
	AMainViewTmpWidget.cpp
	ASlidyWidget.cpp
	ASlidyFlap.cpp
	APlugin.cpp
	AMELIA.cpp
  )

SET(amelia_H
	QGraphicsClickablePixmapItem.h
	QGraphicsClickableSimpleTextItem.h
	QGraphicsItemLayout.h
	QGraphicsItemTransformAnimation.h
	QBetterMimeData.h
	QTimeLineProxy.h
	QBoolSync.h
	QDoubleSync.h
	AAnimationGUI.h
	AAnimationLayoutGUI.h
	AHideAButton.h
	AMainView.h
	AMainViewTmpWidget.h
	ASlidyWidget.h
 	APlugin.h
	AMELIA.h
    )

SET(amelia_FILES amelia.qrc)

QT4_WRAP_CPP(amelia_SRCS ${amelia_H})
QT4_ADD_RESOURCES(amelia_SRCS ${amelia_FILES})

ADD_LIBRARY(amelia SHARED ${amelia_SRCS})
TARGET_LINK_LIBRARIES(amelia
  ${QT_LIBRARIES})
