/**************** License agreement ******************

AMELIA Copyright (c) 2008, The Regents of the University of California,
through Lawrence Berkeley National Laboratory.  All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

(1) Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

(2) Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

(3) Neither the name of the University of California, Lawrence Berkeley National Laboratory,
U.S. Dept. of Energy nor the names of its contributors may be used to endorse or promote products
derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

You are under no obligation whatsoever to provide any bug fixes, patches, or upgrades to the features,
functionality or performance of the source code ("Enhancements") to anyone; however, if you choose to
make your Enhancements available either publicly, or directly to Lawrence Berkeley National Laboratory,
without imposing a separate written license agreement for such Enhancements, then you hereby grant the
following license: a  non-exclusive, royalty-free perpetual license to install, use, modify,
prepare derivative works, incorporate into other computer software, distribute,
and sublicense such enhancements or derivative works thereof, in binary and source code form.
******************************************************/



#ifndef ALAYERGUI_H
#define ALAYERGUI_H

#include <QFrame>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QTabWidget>
#include <QToolBox>
#include <QTableWidget>
#include <QPushButton>
#include <QGraphicsView>
#include <QLabel>
#include <QSignalMapper>
#include <QTreeWidget>
#include <QDialogButtonBox>

#include "AGeometry.h"
#include "AGeometryHUD.h"
#include "AEventInfoScene.h"
#include "ASelectionInfoScene.h"
#include "ATrackTreeModel.h"
#include "AInterestingTrackTableModel.h"
#include "AMainView.h"
#include "AAdvancedEventSettings.h"
#include "ASnapshotTool.h"

#include <QDoubleSync.h>
#include <QFaderWidget.h>

#include <aeventmanager/AEventManagerTreeView.h>
#include <aeventmanager/AEventManagerScene.h>

#include <aeventmanager/APtFilter.h>
#include <aeventmanager/AModelFilter.h>
#include <aeventmanager/AParticleFilter.h>
#include <aeventmanager/ACalorimeterFilter.h>

#include "AGeometryDefines.h"

class AGEOMETRY_EXPORT ALayerGUI : public QFrame
{
public:
  ALayerGUI(QWidget* parent=0);
  virtual ~ALayerGUI();
  void setupElements(AEventManager*);

  /*!
   * Model used to manage the event object bookmarks
   * \return The model used by ageometry to manage bookmarks
   */
  ATrackTreeModel* bookmarkModel();

  /*!
   * Returns the current, unfiltered, event displayed by the ageometry plugin. 0 if none is loaded
   *
   * \return Current event
   */
  AEvent *completeEvent();
  
signals:
  // Emitted when an event has finished loading from the JiveXML file
  void eventLoaded(AEvent*);
  void eventChanged(AEvent*);
  void eventUnloaded();
		      
public slots:
  void actionSwitchView();
  void enableElements();
  
  //Slots for event handling
  void handleCropModeChange(int mode);
  void showLoadEventDialog();
  void eventSettings();
  
  void loadEvent(QString);
  void loadEvent(AEvent*);
  
  void unloadEvent();
  
  //Loaded event, sets up visibility of tables and other funfun stuff
  void handleEventLoaded();
  void handleEventUnloaded();
  
  void handlePackageActivated();
  
  //Slots for event manager
  void handleEventTagChange(bool status);
  
  // Slots for event actions
  void deleteSelectedTreeTracks();
  void combineSelectedTreeTracks();
  void handleTreeSelectionChanged(const QItemSelection&, const QItemSelection&);
  void performTreeTrackSelection(AEventObject*, bool);
  void performTreeTrackDeselection(AEventObject*);
    
  void about();
  
protected:
  void show(QWidget *);
  void hide(QWidget *);
  
private:
  QSignalMapper smartShowMapper;
  QSignalMapper smartHideMapper;
  QSignalMapper flapMapper;
  AEventInfoScene *eventInfo;
  ASelectionInfoScene *selectionInfo;
  
  // Pointers to commonly used widgets
  ATrackTreeModel *_bookmarkModel;
  AInterestingTrackTableModel *interestingTracksModel;
  
  AGeometry* geo;
  AGeometryHUD* hud;
  QTableView *tableInterestingTracks;
  QTreeView *detailedSelectedTracksTable;
  QTreeView *tableSelectedTracks;
  QWidget *eventWidget;
  AMainView *AGeometryFrame;
  QMenu *menuTagCurrentEvent;
  QAction *actionNextEvent;
  QAction *actionTagHiggsBoson;
  QAction *actionTagBlackHole;
  QAction *actionTable;
  QGraphicsView *selectedEventInfoView;
  QPushButton *buttonCombineTracks;
  QPushButton *buttonDeleteTracks;
  
  //Cropping controls
  QActionGroup *croppingModes;
  QAction *actionNone;
  QAction *actionWedge_Mode;
  QAction *actionMoses_Mode;
  QSignalMapper croppingMapper;
  
  // Random stuff
  QActionGroup *cameraModes;
  ASnapshotTool snapshotTool;
  
  QMap<QString,QPoint> widgetPositions;
  QSignalMapper *signalMapper;
  
  //Event manager stuff
  AEventManagerScene *mngr;
  AEventManagerTreeView *packageList;
  
  // Filters
  QDoubleSync ptFilterSync;
  APtFilter *ptFilter;
  AModelFilter *modelFilter;
  ACalorimeterFilter *calorimeterFilter;
  AParticleFilter *particleFilter;
  AEvent* CompleteEvent;
  AFilteredEvent* FilteredEvent;
  AFilteredEvent* ModelEvent;
  
  friend class AGeoPlugin;
  
  void setEventSpecificActionsEnabled(bool status);
  
  Q_OBJECT
};



#endif // ALAYERGUI_H
