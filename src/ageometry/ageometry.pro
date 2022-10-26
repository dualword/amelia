
TEMPLATE = lib
TARGET = ../../bin/AGeometry
DEPENDPATH += . media
INCLUDEPATH += . ../libamelia
include(../../common.pri)

QT += xml opengl
CONFIG += shared plugin

LIBS += -L../../bin/ -lamelia
LIBS += -L../../bin/ -lABase -lAEventManager

HEADERS += AAdvancedEventSettings.h \
           ACaloTower3DNode.h \
           ACameraControl.h \
           AEventInfoScene.h \
           AEventObject3DNode.h \
           AFOVControl.h \
           AFPSControl.h \
           AGeometry.h \
           AGeometryDefines.h \
           AGeometryHUD.h \
           AGeoPlugin.h \
           AHelix3DNode.h \
           AInterestingTrackTableModel.h \
           AJet3DNode.h \
           ALayerGUI.h \
           ALineNode.h \
           AMisET3DNode.h \
           APoint3D.h \
           ARTrack3DNode.h \
           ASelectionInfoScene.h \
           ASnapshotTool.h \
           ASphereControl.h \
           ASTrack3DNode.h \
           ATrackTreeModel.h \
           AViewport.h \
           CRelativeScaleSceneNodeAnimator.h \
           CSceneNodeAnimatorCameraFOV.h \
           CSceneNodeAnimatorCameraOrbit.h \
           CSceneNodeAnimatorCameraSphere.h \
           CSceneNodeAnimatorCameraSwitch.h \
           QAbstractItemModelWithContextMenu.h \
           QAbstractTableModelWithContextMenu.h \
           QAbstractTreeItem.h \
           QIrrWidget.h \
           QIrrWidgetRefresher.h \
           QTableViewWithContextMenu.h \
           QTreeViewWithContextMenu.h

FORMS += detectorvisibility.ui eventadvanced.ui geometry.ui

SOURCES += AAdvancedEventSettings.cpp \
           ACaloTower3DNode.cpp \
           ACameraControl.cpp \
           AEventInfoScene.cpp \
           AEventObject3DNode.cpp \
           AFOVControl.cpp \
           AFPSControl.cpp \
           AGeometry.cpp \
           AGeometryHUD.cpp \
           AGeoPlugin.cpp \
           AHelix3DNode.cpp \
           AInterestingTrackTableModel.cpp \
           AJet3DNode.cpp \
           ALayerGUI.cpp \
           ALineNode.cpp \
           AMisET3DNode.cpp \
           APoint3D.cpp \
           ARTrack3DNode.cpp \
           ASelectionInfoScene.cpp \
           ASnapshotTool.cpp \
           ASphereControl.cpp \
           ASTrack3DNode.cpp \
           ATrackTreeModel.cpp \
           AViewport.cpp \
           CRelativeScaleSceneNodeAnimator.cpp \
           CSceneNodeAnimatorCameraFOV.cpp \
           CSceneNodeAnimatorCameraOrbit.cpp \
           CSceneNodeAnimatorCameraSphere.cpp \
           CSceneNodeAnimatorCameraSwitch.cpp \
           QAbstractItemModelWithContextMenu.cpp \
           QAbstractTableModelWithContextMenu.cpp \
           QAbstractTreeItem.cpp \
           QIrrWidget.cpp \
           QIrrWidgetRefresher.cpp \
           QTableViewWithContextMenu.cpp \
           QTreeViewWithContextMenu.cpp

RESOURCES += ageometry.qrc media/geoskin.qrc


OBJECTS_DIR = .build/obj
MOC_DIR     = .build/moc
RCC_DIR     = .build/rcc
UI_DIR      = .build/ui



