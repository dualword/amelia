FIND_PACKAGE (Qt4 REQUIRED)
INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR})
INCLUDE(${QT_USE_FILE})

SET(speedtest_SRCS
  speedtest.cpp)

SET(eventtest_SRCS
  AShittyHistogram.cpp
  eventtest.cpp)

SET(packagetest_SRCS
  AShittyHistogram.cpp
  packagetest.cpp)

SET(managertest_SRCS
  AShittyHistogram.cpp
  ManagerTestHelper.cpp
  managertest.cpp)

SET(managertest_H
  ManagerTestHelper.h)
QT4_WRAP_CPP(managertest_SRCS ${managertest_H})

ADD_EXECUTABLE(speedtest ${speedtest_SRCS})
ADD_EXECUTABLE(eventtest ${eventtest_SRCS})
ADD_EXECUTABLE(packagetest ${packagetest_SRCS})
ADD_EXECUTABLE(managertest ${managertest_SRCS})

TARGET_LINK_LIBRARIES(speedtest
  ${QT_LIBRARIES} 
  amelia
  AEventManager)

TARGET_LINK_LIBRARIES(eventtest
  ${QT_LIBRARIES} 
  amelia
  AEventManager)

TARGET_LINK_LIBRARIES(packagetest
  ${QT_LIBRARIES} 
  amelia
  AEventManager)

TARGET_LINK_LIBRARIES(managertest
  ${QT_LIBRARIES} 
  amelia
  AEventManager)
