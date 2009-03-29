SET(QT_USE_QTXML 1)

FIND_PACKAGE (Qt4 REQUIRED)
INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR})
INCLUDE(${QT_USE_FILE})


ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)

SET(alabnotebook_SRCS
  ALabNoteBook.cpp
  ALabNoteBookWidget.cpp
  ALabNoteBookTableModel.cpp
  ALabNoteBookEntry.cpp
  ALabNoteBookData.cpp)

SET(alabnotebook_H
  ALabNoteBook.h
  ALabNoteBookWidget.h
  ALabNoteBookTableModel.h
  ALabNoteBookEntry.h
  ALabNoteBookData.h)

SET(alabnotebook_UI
  labnotebook.ui)

SET(alabnotebook_FILES
  alabnotebook.qrc)

QT4_WRAP_CPP(alabnotebook_SRCS ${alabnotebook_H})
QT4_WRAP_UI(alabnotebook_SRCS ${alabnotebook_UI})
QT4_ADD_RESOURCES(alabnotebook_SRCS ${alabnotebook_FILES})

ADD_LIBRARY(ALabNoteBook SHARED ${alabnotebook_SRCS})
LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})
TARGET_LINK_LIBRARIES(ALabNoteBook
  ${QT_LIBRARIES} 
  amelia)
