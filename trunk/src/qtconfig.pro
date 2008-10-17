CONFIG += app uitools debug
QT += opengl
unix:LIBS += -lIrrlicht -lGL -lXxf86vm -lGLU -L/home/jacob/src/libs/irrlicht/trunk/lib/Linux -L/home/kkrizka/Sources/irrlicht/lib/Linux -L/home/kka34/Sources/irrlicht/lib/Linux
win32:LIBS += E:\irrlicht\IrrlichtSVN\lib\Win32-visualstudio\Irrlicht.lib F:\Irrlicht\SVN\lib\Win32-visualstudio\Irrlicht.lib

unix:INCLUDEPATH += /home/jacob/src/libs/irrlicht/trunk/include;/home/kkrizka/Sources/irrlicht/include;/home/kka34/Sources/irrlicht/include

win32:INCLUDEPATH += E:\irrlicht\IrrlichtSVN\include\;F:\Irrlicht\SVN\trunk\include



HEADERS += \
 QIrrWidget.h \
 ABase.h \
 AGeometry.h \
 ATrack3DNode.h \
 AMenu.h \
 ATourBuilder.h \
 ATourManager.h \
 AXmlEvent.h \
 AUILoader.h \
 ALayerGUI.h \
 AViewport.h \
 AEventInfoScene.h \
 ATrackTableModel.h \
 AComboTableModel.h \
 ATrackCombination.h \
 CSceneNodeAnimatorCameraSphere.h\
 AAnimationGUI.h \
 CRelativeScaleSceneNodeAnimator.h\
 ATrack.h\
 AMainView.h


SOURCES += \
 QIrrWidget.cpp \
 ABase.cpp \
 AGeometry.cpp \
 ATrack3DNode.cpp \
 AMenu.cpp \
 ATourBuilder.cpp \
 ATourManager.cpp \
 AXmlEvent.cpp \
 AUILoader.cpp \
 ALayerGUI.cpp \
 AViewport.cpp \
 AEventInfoScene.cpp \
 CSceneNodeAnimatorCameraSphere.cpp\
 ATrackTableModel.cpp \
 AComboTableModel.cpp \
 ATrackCombination.cpp \
 AMainView.cpp \
 AAnimationGUI.cpp \
 CRelativeScaleSceneNodeAnimator.cpp\
 ATrack.cpp\
 main.cpp

RESOURCES     = ../media/ageometry.qrc \
../ui/ui.qrc

OBJECTS_DIR = ../obj
TARGET = ../Amelia

