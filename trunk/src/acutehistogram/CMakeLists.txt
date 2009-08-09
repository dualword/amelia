SET(QT_USE_QTXML true)

FIND_PACKAGE (Qt4 REQUIRED)
INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR})
INCLUDE(${QT_USE_FILE})

ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)

SET(acutehistogram_SRCS
  ACuteHistogram.cpp
  AHistogramWidget.cpp)

SET(acutehistogram_H
  ACuteHistogram.h)

SET(acutehistogram_FILES
  acutehistogram.qrc)

QT4_WRAP_CPP(acutehistogram_SRCS ${acutehistogram_H})
QT4_ADD_RESOURCES(acutehistogram_SRCS ${acutehistogram_FILES})

ADD_LIBRARY(ACuteHistogram SHARED ${acutehistogram_SRCS})
LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})
TARGET_LINK_LIBRARIES(ACuteHistogram
  ${QT_LIBRARIES} 
  amelia)
