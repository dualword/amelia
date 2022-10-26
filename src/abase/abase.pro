TEMPLATE = lib
TARGET = ../../bin/ABase
DEPENDPATH += . media
INCLUDEPATH += . ../libamelia/

QT += xml opengl
CONFIG += shared plugin

LIBS += -L../../bin/ -lamelia

HEADERS += ABase.h ABaseDefines.h AMonitor.h AMonitorGroup.h
SOURCES += ABase.cpp AMonitor.cpp AMonitorGroup.cpp
RESOURCES += media/media.qrc

OBJECTS_DIR = .build/obj
MOC_DIR     = .build/moc
RCC_DIR     = .build/rcc
UI_DIR      = .build/ui


