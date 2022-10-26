TEMPLATE = lib
TARGET = ../../bin/AEventManager
DEPENDPATH += . media
INCLUDEPATH += . .. ../libamelia/
include(../../common.pri)

QT += xml opengl
CONFIG += shared plugin

LIBS += -L../../bin/ -lamelia

HEADERS += ACalorimeterFilter.h \
           AEvent.h \
           AEventAnalysisData.h \
           AEventManager.h \
           AEventManagerDefines.h \
           AEventManagerScene.h \
           AEventManagerTreeView.h \
           AEventObject.h \
           AEventObjectCollection.h \
           AEventObjectFilter.h \
           AEventPackage.h \
           AFCALShower.h \
           AFilteredEvent.h \
           AJet.h \
           AMisET.h \
           AModelFilter.h \
           AParticleFilter.h \
           APtFilter.h \
           ARTrack.h \
           AShower.h \
           ASTrack.h \
           ATrack.h \
           ATrackCombination.h \
           AXmlEvent.h \
           #tests/AShittyHistogram.h \
           #tests/ManagerTestHelper.h \
           config.h

SOURCES += ACalorimeterFilter.cpp \
           AEvent.cpp \
           AEventAnalysisData.cpp \
           AEventManager.cpp \
           AEventManagerScene.cpp \
           AEventManagerTreeView.cpp \
           AEventObject.cpp \
           AEventObjectCollection.cpp \
           AEventObjectFilter.cpp \
           AEventPackage.cpp \
           AFCALShower.cpp \
           AFilteredEvent.cpp \
           AJet.cpp \
           AMisET.cpp \
           AModelFilter.cpp \
           AParticleFilter.cpp \
           APtFilter.cpp \
           ARTrack.cpp \
           AShower.cpp \
           ASTrack.cpp \
           ATrack.cpp \
           ATrackCombination.cpp \
           AXmlEvent.cpp
           #tests/AShittyHistogram.cpp \
           #tests/eventtest.cpp \
           #tests/managertest.cpp \
           #tests/ManagerTestHelper.cpp \
           #tests/packagetest.cpp \
           #tests/speedtest.cpp

OBJECTS_DIR = .build/obj
MOC_DIR     = .build/moc
RCC_DIR     = .build/rcc
UI_DIR      = .build/ui


