TEMPLATE = lib
TARGET = ../../bin/amelia
DEPENDPATH += .
INCLUDEPATH += .

QT += xml opengl
CONFIG += shared

HEADERS += AAnimationGUI.h \
           AAnimationLayoutGUI.h \
           ADefines.h \
           AHideAButton.h \
           AMainView.h \
           AMainViewTmpWidget.h \
           AMELIA.h \
           APlugin.h \
           ASlidyManager.h \
           ASlidyWidget.h \
           QBetterMimeData.h \
           QBoolSync.h \
           QDoubleSync.h \
           QFaderWidget.h \
           QGraphicsClickablePixmapItem.h \
           QGraphicsClickableSimpleTextItem.h \
           QGraphicsItemLayout.h \
           QGraphicsItemTransformAnimation.h \
           QTimeLineProxy.h \
           QWaitForReady.h

SOURCES += AAnimationGUI.cpp \
           AAnimationLayoutGUI.cpp \
           AHideAButton.cpp \
           AMainView.cpp \
           AMainViewTmpWidget.cpp \
           AMELIA.cpp \
           APlugin.cpp \
           ASlidyManager.cpp \
           ASlidyWidget.cpp \
           QBetterMimeData.cpp \
           QBoolSync.cpp \
           QDoubleSync.cpp \
           QFaderWidget.cpp \
           QGraphicsClickablePixmapItem.cpp \
           QGraphicsClickableSimpleTextItem.cpp \
           QGraphicsItemLayout.cpp \
           QGraphicsItemTransformAnimation.cpp \
           QTimeLineProxy.cpp \
           QWaitForReady.cpp
RESOURCES += amelia.qrc

OBJECTS_DIR = .build/obj
MOC_DIR     = .build/moc
RCC_DIR     = .build/rcc
UI_DIR      = .build/ui


