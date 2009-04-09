FIND_PROGRAM(ROOTCONFIG NAMES root-config)
MESSAGE(STATUS ${ROOTCONFIG})
IF(NOT ROOTCONFIG)
  MESSAGE(STATUS  "root-config not found")
ELSE(NOT ROOTCONFIG)
  
  EXEC_PROGRAM(${ROOTCONFIG} ARGS "--has-qt" OUTPUT_VARIABLE ROOTWITHQT)

  IF(NOT ROOTWITHQT)
    MESSAGE("ROOT not compiled with Qt")
  ELSE(NOT ROOTWITHQT)
    EXEC_PROGRAM(${ROOTCONFIG}
      ARGS "--libs"
      OUTPUT_VARIABLE ROOT_LIBS )
    STRING(REGEX MATCHALL "-l[^ ]+" ROOT_LIBS_LIST "${ROOT_LIBS}" )
    STRING(REGEX REPLACE "-l" "" ROOT_LIBRARIES "${ROOT_LIBS_LIST}" )
    EXEC_PROGRAM(${ROOTCONFIG}
      ARGS "--libdir"
      OUTPUT_VARIABLE ROOT_LIB_DIRECTORIES )
    EXEC_PROGRAM(${ROOTCONFIG}
      ARGS "--incdir"
      OUTPUT_VARIABLE ROOT_INCLUDE_DIR )
MESSAGE(STATUS ${ROOT_LIBS})
    SET(QT_USE_QTXML 1)
    
    FIND_PACKAGE (Qt4 REQUIRED)
    INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR} ${ROOT_INCLUDE_DIR})
    INCLUDE(${QT_USE_FILE})
    
    
    ADD_DEFINITIONS(${QT_DEFINITIONS})
    ADD_DEFINITIONS(-DQT_PLUGIN)
    ADD_DEFINITIONS(-DQT_NO_DEBUG)
    ADD_DEFINITIONS(-DQT_SHARED)
    
    SET(ahist_SRCS
      AHistPlugin.cpp
      ATest.cpp
      AllHistograms.cpp
      AComboHistogram.cpp)
    
    SET(ahist_H
      AHistPlugin.h
      ATest.h
      AllHistograms.h
      AComboHistogram.h)

    SET(ahist_UI
      histograms.ui
      allhistograms.ui
      acombohistogram.ui)
    
    SET(ahist_FILES
      ahist.qrc)
    
    QT4_WRAP_CPP(ahist_SRCS ${ahist_H})
    QT4_WRAP_UI(ahist_SRCS ${ahist_UI})
    QT4_ADD_RESOURCES(ahist_SRCS ${ahist_FILES})
    
    LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH} ${ROOT_LIB_DIRECTORIES})
    ADD_LIBRARY(AHist SHARED ${ahist_SRCS})
    TARGET_LINK_LIBRARIES(AHist
      ${QT_LIBRARIES} 
      ${ROOT_LIBRARIES}
      QtRoot
      GQt
      Gui
      amelia
      ABase)
    ENDIF(NOT ROOTWITHQT)
ENDIF(NOT ROOTCONFIG)

