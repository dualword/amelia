SET(QT_USE_QTWEBKIT 1)

FIND_PACKAGE (Qt4 REQUIRED)
INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR})
INCLUDE(${QT_USE_FILE})


ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_SHARED)

SET(aweb_SRCS
  AWebPlugin.cpp
  AWebView.cpp
  ADropDownMenu.cpp)

SET(aweb_H
  AWebPlugin.h
  AWebView.h
  ADropDownMenu.h)

SET(aweb_UI
  browser.ui)

SET(aweb_FILES
  plugin.qrc)

QT4_WRAP_UI(aweb_SRCS ${aweb_UI})

QT4_WRAP_CPP(aweb_SRCS ${aweb_H})
QT4_ADD_RESOURCES(aweb_SRCS ${aweb_FILES})

ADD_LIBRARY(AWeb SHARED ${aweb_SRCS})
LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})
TARGET_LINK_LIBRARIES(AWeb
  ${QT_LIBRARIES} 
  amelia
  ABase)

INSTALL(TARGETS AWeb
  DESTINATION lib/amelia/plugins)