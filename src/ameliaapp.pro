TEMPLATE = app
TARGET = ../bin/AmeliaApp
DEPENDPATH += .
INCLUDEPATH += . ../bin/

QT += 
CONFIG += qt

QMAKE_CXXFLAGS += -I./libamelia/
LIBS += -L../bin/ -lamelia

HEADERS += config.h
SOURCES += main.cpp

RESOURCES += 

OBJECTS_DIR = .build/obj
MOC_DIR     = .build/moc
RCC_DIR     = .build/rcc
UI_DIR      = .build/ui


