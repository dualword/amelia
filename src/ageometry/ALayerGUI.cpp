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

#include <QLayout>
#include <QMenu>
#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QCheckBox>
#include <QSortFilterProxyModel>
#include <QDebug>

#include "ALayerGUI.h"
#include "AViewport.h"

#include "ui_eventadvanced.h"
#include "ui_detectorvisibility.h"

#include <config.h>
#include <AAnimationGUI.h>
#include <AAnimationLayoutGUI.h>
#include <ASlidyManager.h>
#include <aeventmanager/AXmlEvent.h>

ALayerGUI::ALayerGUI(QWidget* parent)
        : QFrame(parent)
{
    modelFilter=new AModelFilter(AJet::jKt4H1TopoJets,AMisET::mMET_Final);
    particleFilter=new AParticleFilter();
    ptFilter=new APtFilter(1,particleFilter);
}

ALayerGUI::~ALayerGUI()
{ }

void ALayerGUI::setupElements(AEventManager *eventmanager)
{
    qDebug() << "Setting up elements";

    //Find all required widgets...
    tableSelectedTracks = findChild<QTreeView*>("selectedTracksTable");
    tableInterestingTracks = findChild<QTableView*>("interestingTracksTable");
    geo=findChild<AGeometry*>("Geometry");
    AViewport* LeftViewport=findChild<AViewport*>("LeftViewport");
    AViewport* RightViewport=findChild<AViewport*>("RightViewport");
    menuTagCurrentEvent=window()->findChild<QMenu *>("menuTagCurrentEvent");
    buttonDeleteTracks = findChild<QPushButton*>("deleteTracks");
    buttonCombineTracks = findChild<QPushButton*>("combineTracks");
    QGraphicsView *trackInfoView=findChild<QGraphicsView *>("trackInfo");
    QGraphicsView *eventInfoView=findChild<QGraphicsView *>("eventInfo");
    detailedSelectedTracksTable=findChild<QTreeView *>("detailedSelectedTracksTable");
    QAction *actionFPS=window()->findChild<QAction *>("actionFPS");
    QAction *actionSphere=window()->findChild<QAction *>("actionSphere");
    QAction *actionSnapshot=window()->findChild<QAction *>("actionSnapshot");
    actionNextEvent=window()->findChild<QAction *>("actionNextEvent");
    actionTable=window()->findChild<QAction *>("actionTable");
    actionNone=window()->findChild<QAction *>("actionNone");
    actionWedge_Mode=window()->findChild<QAction *>("actionWedge_Mode");
    actionMoses_Mode=window()->findChild<QAction *>("actionMoses_Mode");
    QPushButton *nextEventButton = findChild<QPushButton*>("nextEventButton");
    QSlider *PtCutoff_Slider=findChild<QSlider*>("PtCutoff_Slider");
    QDoubleSpinBox *spinBox_Pt=findChild<QDoubleSpinBox*>("spinBox_Pt");
    actionTagHiggsBoson=window()->findChild<QAction *>("actionTagHiggsBoson");
    actionTagBlackHole=window()->findChild<QAction *>("actionTagBlackHole");
    QPushButton *button_detectorVisibility=findChild<QPushButton *>("button_detectorVisibility");
    QPushButton *button_packageList=findChild<QPushButton *>("button_packageList");
    QPushButton *button_eventInfo=findChild<QPushButton *>("button_eventInfo");

    QWidget *detectorVisibility=new QWidget();
    Ui::DetectorVisibility ui;
    ui.setupUi(detectorVisibility);
    
    QGroupBox *groupBox_Detector=detectorVisibility->findChild<QGroupBox *>("groupBox_Detector");
    QGroupBox *groupBox_Calorimeter=detectorVisibility->findChild<QGroupBox *>("groupBox_Calorimeter");
    QGroupBox *groupBox_ID=detectorVisibility->findChild<QGroupBox *>("groupBox_ID");
    QCheckBox *checkBox_Pit=detectorVisibility->findChild<QCheckBox *>("checkBox_Pit");
    QCheckBox *checkBox_Magnets=detectorVisibility->findChild<QCheckBox *>("checkBox_Magnets");
    QCheckBox *checkBox_MuonSpectr=detectorVisibility->findChild<QCheckBox *>("checkBox_MuonSpectr");
    QCheckBox *checkBox_LAr=detectorVisibility->findChild<QCheckBox *>("checkBox_LAr");
    QCheckBox *checkBox_Tilecal=detectorVisibility->findChild<QCheckBox *>("checkBox_Tilecal");
    QCheckBox *checkBox_Pixels=detectorVisibility->findChild<QCheckBox *>("checkBox_Pixels");
    QCheckBox *checkBox_SCT=detectorVisibility->findChild<QCheckBox *>("checkBox_SCT");
    QCheckBox *checkBox_TRT=detectorVisibility->findChild<QCheckBox *>("checkBox_TRT");
    
    QCheckBox *checkBox_Electrons=findChild<QCheckBox *>("checkBox_Electrons");
    QCheckBox *checkBox_NeutralHadrons=findChild<QCheckBox *>("checkBox_NeutralHadrons");
    QCheckBox *checkBox_Muons=findChild<QCheckBox *>("checkBox_Muons");
    QCheckBox *checkBox_Photons=findChild<QCheckBox *>("checkBox_Photons");
    QCheckBox *checkBox_Jets=findChild<QCheckBox *>("checkBox_Jets");
    QCheckBox *checkBox_MissingEt=findChild<QCheckBox *>("checkBox_MissingEt");

    eventWidget=findChild<QWidget *>("eventWidget");
    AGeometryFrame=findChild<AMainView *>("AGeometryFrame");

    tracksModel=new ATrackTreeModel(this);
    connect(this,SIGNAL(eventLoaded(AEvent*)),
	    tracksModel,SLOT(handleNewEventLoaded(AEvent*)));	    

    interestingTracksModel=new AInterestingTrackTableModel(this);

    //Setup Tables
    if (tableSelectedTracks)
      {
	tracksModel->addView(tableSelectedTracks);

	tableSelectedTracks->setColumnWidth ( 0, 110 );
        tableSelectedTracks->setColumnWidth ( 1,  50 );

	tableSelectedTracks->hideColumn(2);
	tableSelectedTracks->hideColumn(3);
	tableSelectedTracks->hideColumn(4);

	connect(tableSelectedTracks->selectionModel(),SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)),
		this,SLOT(handleTreeSelectionChanged(const QItemSelection&,const QItemSelection&)));
    

      }
    if (detailedSelectedTracksTable)
      {
        tracksModel->addView(detailedSelectedTracksTable);

	detailedSelectedTracksTable->hideColumn(0);
	detailedSelectedTracksTable->hideColumn(1);
      }

    if (tableInterestingTracks)
    {
        interestingTracksModel->addTable(tableInterestingTracks);

        tableInterestingTracks->setColumnWidth ( 0, 60 );
        tableInterestingTracks->setColumnWidth ( 1, 60 );
        tableInterestingTracks->setColumnWidth ( 2, 70 );

        connect(interestingTracksModel,SIGNAL(entrySelected(unsigned int,bool)),
                geo,SLOT(selectTrackByID(unsigned int,bool)));
        connect(interestingTracksModel,SIGNAL(entryDeselected(unsigned int)),
                geo,SLOT(deselectTrackByID(unsigned int)));
    }

    // Setup trackview
    if (trackInfoView)
    {
        trackInfo=new ASelectionInfoScene;
        trackInfoView->setScene(trackInfo);
        trackInfoView->ensureVisible(0,0,450,300,10,10);
	connect(this,SIGNAL(eventLoaded(AEvent*)),
		trackInfo,SLOT(handleNewEventLoaded(AEvent*)));

        //Signals...
        connect(geo,SIGNAL(trackSelected(ATrack*)), //track selection
                trackInfo,SLOT(updateTrackInfo(ATrack*)));
        connect(geo,SIGNAL(trackDeselected(ATrack*)), //track selection
                trackInfo,SLOT(removeTrackInfo(ATrack*)));
        connect(geo,SIGNAL(emptySelection()),
                trackInfo,SLOT(hideMessage()));
    }

    eventInfoView=new QGraphicsView();
    eventInfo=new AEventInfoScene;
    eventInfoView->resize(300,200);
    eventInfoView->setScene(eventInfo);
    eventInfoView->ensureVisible(0,0,450,300,10,10);

    // Setup package list
    mngr=new AEventManagerScene(eventmanager,"AGeometry");
    packageList=new AEventManagerTreeView();
    packageList->setModel(mngr);
    packageList->setColumnWidth(0,200);
    
    connect(packageList, SIGNAL(eventClicked ( AEvent* )),
	    this, SLOT(loadEvent( AEvent* )));
    connect(packageList, SIGNAL(packageActivated ( AEventPackage* )),
	    this, SLOT(handlePackageActivated( )));
    connect(this, SIGNAL(eventLoaded(AEvent*)),
	    mngr, SLOT(setActiveEvent(AEvent*)));
    connect(this,SIGNAL(eventUnloaded()),
	    mngr, SLOT(setActiveEvent()));
    
    //Setup slide widget
    ASlidyManager *slide=new ASlidyManager(this,250);
    slide->setObjectName("AGeoControl");
    slide->setPosition(125);
    slide->addWidget(eventInfoView,"Event Info",true);
    slide->addWidget(packageList,"Packages",true);
    slide->addWidget(detectorVisibility,"Detector Visibility",true);
    connect(&flapMapper,SIGNAL(mapped(int)),
	    slide,SLOT(toggleWidget(int)));
    flapMapper.setMapping(button_eventInfo,0);
    connect(button_eventInfo,SIGNAL(clicked()),
	    &flapMapper,SLOT(map()));
    flapMapper.setMapping(button_packageList,1);
    connect(button_packageList,SIGNAL(clicked()),
	    &flapMapper,SLOT(map()));
    flapMapper.setMapping(button_detectorVisibility,2);
    connect(button_detectorVisibility,SIGNAL(clicked()),
	    &flapMapper,SLOT(map()));


    // Setup random buttons
    if (buttonDeleteTracks)
      {
        connect(buttonDeleteTracks,SIGNAL(clicked()),
                this,SLOT(deleteSelectedTreeTracks()));
      }
    if(buttonCombineTracks)
      {
        connect(buttonCombineTracks,SIGNAL(clicked()), //The combine button...
                this,SLOT(combineSelectedTreeTracks()));
      }


    // Setup the next event buttons
    if (actionNextEvent)
      {
        connect(actionNextEvent,SIGNAL(triggered()),
                packageList,SLOT(clickNextEvent()));
      }
    if (nextEventButton)
      {
        connect(nextEventButton,SIGNAL(clicked()),
                packageList,SLOT(clickNextEvent()));
      }
    
    // Setup pt cutoff
    ptFilterSync.setValue(1);
    connect(&ptFilterSync,SIGNAL(valueChanged(double)),
	    ptFilter,SLOT(setMinPt(double)));
    connect(ptFilter,SIGNAL(minPtChanged(double)),
	    &ptFilterSync,SLOT(setValue(double)));
    ptFilterSync.syncSlider(PtCutoff_Slider);
    ptFilterSync.syncSpinBox(spinBox_Pt);

    if (eventWidget)
      {
        eventWidget->hide();
      }

    if (geo) //Setup signals pertaining to AGeometry
      {
        signalMapper=new QSignalMapper(this);
        signalMapper->setMapping(actionSphere, AGeometry::Sphere);
        signalMapper->setMapping(actionFPS, AGeometry::FPS);

        connect(actionSphere,SIGNAL(triggered()),
                signalMapper, SLOT(map()));
        connect(actionFPS,SIGNAL(triggered()),
                signalMapper, SLOT(map()));
        connect(signalMapper, SIGNAL(mapped(int)),
                geo, SLOT(setCamera(int)));
	connect(this,SIGNAL(eventUnloaded()),
		geo,SLOT(clearTrackSelection()));
	connect(geo,SIGNAL(finishedLoading()),
		this,SLOT(enableElements()));
	
	//Setup camera controls and toggles
	cameraModes=new QActionGroup(this);
	cameraModes->addAction(actionFPS);
	cameraModes->addAction(actionSphere);
	geo->actFPS = actionFPS;
	geo->actSphere = actionSphere;

	//Setup event load/update/unload connections
	connect(this,SIGNAL(eventUnloaded()),
		geo,SLOT(clearEvent()));

	hud=new AGeometryHUD(geo);
      }

    if(LeftViewport)
      LeftViewport->setViewport(AGeometry::Front);
    if(RightViewport)
      RightViewport->setViewport(AGeometry::Side);
    
    //Connect the action tags
    connect(actionTagBlackHole,SIGNAL(toggled(bool)),
	    this, SLOT(handleEventTagChange(bool)));
    connect(actionTagHiggsBoson,SIGNAL(toggled(bool)),
	    this, SLOT(handleEventTagChange(bool)));

    //Setup cropping modes
    croppingModes=new QActionGroup(this);
    croppingModes->addAction(actionNone);
    croppingModes->addAction(actionWedge_Mode);
    croppingModes->addAction(actionMoses_Mode);
    croppingMapper.setMapping(actionNone,AGeometry::NoneMode);
    croppingMapper.setMapping(actionWedge_Mode,AGeometry::WedgeMode);
    croppingMapper.setMapping(actionMoses_Mode,AGeometry::MosesMode);
    connect(actionNone,SIGNAL(triggered()),
	    &croppingMapper,SLOT(map()));
    connect(actionWedge_Mode,SIGNAL(triggered()),
	    &croppingMapper,SLOT(map()));
    connect(actionMoses_Mode,SIGNAL(triggered()),
	    &croppingMapper,SLOT(map()));
    connect(&croppingMapper,SIGNAL(mapped(int)),
	    geo,SLOT(setCropMode(int)));
    connect(geo,SIGNAL(cropModeSwitched(int)),
	    this,SLOT(handleCropModeChange(int)));
    
    //Setup visibility toggles
    if(groupBox_Detector)
      geo->detectorVisibility()->syncGroupBox(groupBox_Detector);
    if(groupBox_Calorimeter)
      geo->calorimetersVisibility()->syncGroupBox(groupBox_Calorimeter);
    if(groupBox_ID)
      geo->trackerVisibility()->syncGroupBox(groupBox_ID);

    if(checkBox_Pit)
      geo->pitVisibility()->syncButton(checkBox_Pit);
    if(checkBox_Magnets)
      geo->magnetsVisibility()->syncButton(checkBox_Magnets);
    if(checkBox_MuonSpectr)
      geo->muonSpecVisibility()->syncButton(checkBox_MuonSpectr);
    if(checkBox_LAr)
      geo->larVisibility()->syncButton(checkBox_LAr);
    if(checkBox_Tilecal)
      geo->tileVisibility()->syncButton(checkBox_Tilecal);
    if(checkBox_Pixels)
      geo->pixelsVisibility()->syncButton(checkBox_Pixels);
    if(checkBox_SCT)
      geo->sctVisibility()->syncButton(checkBox_SCT);
    if(checkBox_TRT)
      geo->trtVisibility()->syncButton(checkBox_TRT);

    //Setup particle visiblility toggles
    if(checkBox_Electrons)
      {
	QBoolSync *electronSync=new QBoolSync(checkBox_Electrons->isChecked());
	electronSync->syncButton(checkBox_Electrons);
	connect(electronSync,SIGNAL(valueChanged(bool)),
		particleFilter,SLOT(setShowElectrons(bool)));
	connect(particleFilter,SIGNAL(showElectronsChanged(bool)),
		electronSync,SLOT(setValue(bool)));
      }
    if(checkBox_Muons)
      {
	QBoolSync *muonSync=new QBoolSync(checkBox_Muons->isChecked());
	muonSync->syncButton(checkBox_Muons);
	connect(muonSync,SIGNAL(valueChanged(bool)),
		particleFilter,SLOT(setShowMuons(bool)));
	connect(particleFilter,SIGNAL(showMuonsChanged(bool)),
		muonSync,SLOT(setValue(bool)));
      }
    if(checkBox_Photons)
      {
	QBoolSync *photonSync=new QBoolSync(checkBox_Photons->isChecked());
	photonSync->syncButton(checkBox_Photons);
	connect(photonSync,SIGNAL(valueChanged(bool)),
		particleFilter,SLOT(setShowPhotons(bool)));
	connect(particleFilter,SIGNAL(showPhotonsChanged(bool)),
		photonSync,SLOT(setValue(bool)));
      }
    if(checkBox_Jets)
      {
	QBoolSync *jetSync=new QBoolSync(checkBox_Jets->isChecked());
	jetSync->syncButton(checkBox_Jets);
	connect(jetSync,SIGNAL(valueChanged(bool)),
		particleFilter,SLOT(setShowJets(bool)));
	connect(particleFilter,SIGNAL(showJetsChanged(bool)),
		jetSync,SLOT(setValue(bool)));
      }
    if(checkBox_NeutralHadrons)
      {
	QBoolSync *hadronsSync=new QBoolSync(checkBox_NeutralHadrons->isChecked());
	hadronsSync->syncButton(checkBox_NeutralHadrons);
	connect(hadronsSync,SIGNAL(valueChanged(bool)),
		particleFilter,SLOT(setShowHadrons(bool)));
	connect(particleFilter,SIGNAL(showHadronsChanged(bool)),
		hadronsSync,SLOT(setValue(bool)));
      }
    if(checkBox_MissingEt)
      {
	QBoolSync *metSync=new QBoolSync(checkBox_MissingEt->isChecked());
	metSync->syncButton(checkBox_MissingEt);
	connect(metSync,SIGNAL(valueChanged(bool)),
		particleFilter,SLOT(setShowMissingEt(bool)));
	connect(particleFilter,SIGNAL(showMissingEtChanged(bool)),
		metSync,SLOT(setValue(bool)));
      }



    //Setup the smart shower
    connect(&smartShowMapper,SIGNAL(mapped(int)),
	    slide,SLOT(showWidgetTimed(int)));
    connect(&smartHideMapper,SIGNAL(mapped(int)),
	    slide,SLOT(hideWidget(int)));
    
    smartShowMapper.setMapping(&ptFilterSync,0);
    connect(&ptFilterSync,SIGNAL(valueChanged(double)),
	    &smartShowMapper,SLOT(map()));

    //Prepare the snapshot controls
    snapshotTool.setTarget(geo);
    connect(actionSnapshot,SIGNAL(triggered()),
	    &snapshotTool,SLOT(snapshot()));
}

void ALayerGUI::enableElements()
{
  QList<QWidget*> children=findChildren<QWidget*>();
  for(int i=0;i<children.size();i++)
    if(children[i]->parentWidget()==this)
      children[i]->setEnabled(true);
  
  QWidget *window=this->parentWidget();
  if(window->inherits("QMainWindow"))
    {
      QMainWindow *mainwindow=(QMainWindow*)window;
      mainwindow->menuBar()->setEnabled(true);

      QList<QToolBar*> toolbars=mainwindow->findChildren<QToolBar*>();
      for(int i=0;i<toolbars.size();i++)
	toolbars[i]->setEnabled(true);      
    }
}

//slots for menus
void ALayerGUI::actionSwitchView()
{
  QString from=sender()->objectName();

  if (from=="actionFirst_Person")
    {
      geo->setViewport(AGeometry::Cam3D);
    }
  else if (from=="actionFront")
    {
      geo->setViewport(AGeometry::Front);
    }
  else if (from=="actionSide")
    {
      geo->setViewport(AGeometry::Side);
    }
}

void ALayerGUI::handleCropModeChange(int mode)
{
  switch(mode)
    {
    case AGeometry::NoneMode:
      actionNone->setChecked(true);
      break;
    case AGeometry::WedgeMode:
      actionWedge_Mode->setChecked(true);
      break;
    case AGeometry::MosesMode:
      actionMoses_Mode->setChecked(true);
      break;
    }
}


void ALayerGUI::showLoadEventDialog()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Event"), "./media/events/", tr("Event Files (*.xml)"));

    loadEvent(fileName);
}

void ALayerGUI::loadEvent(QString fileName)
{
  qDebug() << "Loading " << fileName << endl;

  //TODO Error somehow
  if (!geo) return;

  if (!fileName.isEmpty())
    {
      QFile eventFile(fileName);

      if (!eventFile.exists())
        {
	  QMessageBox::warning(window(), tr("Application"),
			       tr("Cannot find event file: %1!\n.")
			       .arg(fileName));
	  return;
        }


      AXmlEvent *event=new AXmlEvent(fileName);
      loadEvent(event);
    }
}

void ALayerGUI::loadEvent(AEvent* event)
{
  QApplication::setOverrideCursor(Qt::WaitCursor);
  emit eventUnloaded();
  event->LoadEvent();
  CompleteEvent=event;
  ModelEvent=new AFilteredEvent(event,modelFilter);
  FilteredEvent=new AFilteredEvent(ModelEvent,ptFilter);

  handleEventLoaded();

  emit eventLoaded(event);
}

void ALayerGUI::unloadEvent()
{
  handleEventUnloaded();

  emit eventLoaded(0);
  emit eventUnloaded();
}

void ALayerGUI::handleEventLoaded()
{
  //Enable what needs to be enabled!
  setEventSpecificActionsEnabled(true);

  if (interestingTracksModel) interestingTracksModel->setEvent(ModelEvent);
  if (geo) geo->setEvent(FilteredEvent);
  if (eventInfo) eventInfo->setEvent(FilteredEvent);

  if (!eventWidget->isVisible())
    show(eventWidget);

  QApplication::restoreOverrideCursor();
}

void ALayerGUI::handleEventUnloaded()
{
  menuTagCurrentEvent->setEnabled(false);

  setEventSpecificActionsEnabled(false);

  if (eventWidget->isVisible())
    hide(eventWidget);
}

void ALayerGUI::handlePackageActivated()
{
  if (actionNextEvent) actionNextEvent->setEnabled(true);
}

void ALayerGUI::setEventSpecificActionsEnabled(bool status)
{
  if (tableSelectedTracks) tableSelectedTracks->setEnabled(status);
  if (detailedSelectedTracksTable) detailedSelectedTracksTable->setEnabled(status);
  if (buttonDeleteTracks) buttonDeleteTracks->setEnabled(status);
  if (buttonCombineTracks && !status) buttonCombineTracks->setEnabled(status);
  if (actionTable) actionTable->setEnabled(status);
  if (menuTagCurrentEvent) menuTagCurrentEvent->setEnabled(status);
  if (actionTagHiggsBoson) actionTagHiggsBoson->setChecked(status && CompleteEvent->tags().contains("higgs"));
  if (actionTagBlackHole) actionTagBlackHole->setChecked(status && CompleteEvent->tags().contains("blackhole"));
}

void ALayerGUI::eventSettings()
{
  Ui::Dialog ui;
  AAdvancedEventSettings *evSettings=new AAdvancedEventSettings(this);//= loader.load(&file,this);
  ui.setupUi(evSettings);
  evSettings->setModelFilter(modelFilter);
  
  QDialogButtonBox* buttonBox = evSettings->findChild<QDialogButtonBox*>("buttonBox");
  
  connect(buttonBox, SIGNAL(rejected()), evSettings, SLOT(close()));
  connect(buttonBox, SIGNAL(accepted()), evSettings, SLOT(close()));
  
  evSettings->exec();
}

void ALayerGUI::show(QWidget *w)
{
  if (!w) return;

  setUpdatesEnabled(false);
  w->show(); //Need to show the widget to calculate the layout position. It will not be displayed, because updates are disabled.
  QPoint hiddenPosition=w->pos();
  w->hide();
  hiddenPosition.setX(-w->width()-100); //Move it 100 off screen

  QTimeLine *animTime=new QTimeLine(1000,this);
  AAnimationLayoutGUI *anim=new AAnimationLayoutGUI(w,this);
  anim->setTimeLine(animTime);
  anim->setPosAt(0,hiddenPosition);
  //Final position is calculated automatically by AAnimationLayoutGUI as where it should be in the layout..
  connect(animTime,SIGNAL(finished()),
	  anim,SLOT(deleteLater()));
  connect(animTime,SIGNAL(finished()),
	  animTime,SLOT(deleteLater()));

  animTime->start();
  setUpdatesEnabled(true);
}

void ALayerGUI::hide(QWidget *w)
{
  if (!w) return;

  QPoint hiddenPosition=w->pos();
  hiddenPosition.setX(-w->width()-100); //Move it 100 off screen

  QTimeLine *animTime=new QTimeLine(1000,this);
  animTime->setDirection(QTimeLine::Backward);

  AAnimationLayoutGUI *anim=new AAnimationLayoutGUI(w,this);
  anim->setTimeLine(animTime);
  anim->setPosAt(0,hiddenPosition);
  //Final position is calculated automatically by AAnimationLayoutGUI as where it should be in the layout..
  connect(animTime,SIGNAL(finished()),
	  anim,SLOT(deleteLater()));
  connect(animTime,SIGNAL(finished()),
	  animTime,SLOT(deleteLater()));

  animTime->start();
}

void ALayerGUI::handleEventTagChange(bool status)
{
  if(!geo->event()) return;

  QAction *from=(QAction*)sender();
  QString tag;
  if (from==actionTagHiggsBoson)
    tag="higgs";
  else
    tag="blackhole";

  if (status)
    geo->event()->tag(tag,status);
}

void ALayerGUI::combineSelectedTreeTracks()
{
  ATrackCombination *combo=new ATrackCombination(); //hold the stuff
  QItemSelectionModel *selection=tableSelectedTracks->selectionModel();
  QSortFilterProxyModel *sortModel=qobject_cast<QSortFilterProxyModel*>(tableSelectedTracks->model());

  //Get rows. Since all colums should be selected, might as well select column 0.
  QModelIndexList rows=selection->selectedRows();
  //Go through each row...
  for (int i=0;i<rows.size();i++)
    {
      // Make sure to get the index to the correct model, and not the sort proxy
      QModelIndex index;
      if(sortModel)
	index=sortModel->mapToSource(rows[i]);
      else
	index=rows[i];
      
      QAbstractTreeItem *item=(QAbstractTreeItem*)index.internalPointer();
      ATrack *track=qobject_cast<ATrack*>(item->data());
      combo->addTrack(track);
    }
  
  ATrackCollection *analysisData=CompleteEvent->getAnalysisData<ATrackCollection>("AGeometry");
  analysisData->addTrack(combo);
}

void ALayerGUI::deleteSelectedTreeTracks()
{
  QItemSelectionModel *selection=tableSelectedTracks->selectionModel();
  QSortFilterProxyModel *sortModel=qobject_cast<QSortFilterProxyModel*>(tableSelectedTracks->model());

  //Get rows. Since all colums should be selected, might as well select column 0.
  QModelIndexList rows=selection->selectedRows(0);
  //Go through each row...
  for(int i=rows.size()-1;i>=0;i--)
    {
      QModelIndex index;
      // Make sure to get the index to the correct model, and not the sort proxy
      if(sortModel)
	index=sortModel->mapToSource(rows[i]);
      else
	index=rows[i];

      tracksModel->removeRow(index.row(),index.parent());
    }
}

void ALayerGUI::handleTreeSelectionChanged(const QItemSelection& proxyselected,const QItemSelection& proxydeselected)
{
  QSortFilterProxyModel *sortModel=qobject_cast<QSortFilterProxyModel*>(tableSelectedTracks->model());
  QItemSelection selected;
  QItemSelection deselected;
  if(sortModel)
    {
      selected=sortModel->mapSelectionToSource(proxyselected);
      deselected=sortModel->mapSelectionToSource(proxydeselected);
    }
  else
    {
      selected=proxyselected;
      deselected=proxydeselected;
    }

  // PERFORM DESELECTION
  //Deselection should be performed first, because in the case of singletrack selection, everything will be deselected anyways. Even though in theory
  //there shouldn't be any problems, might as well do this to keep safe.
  QModelIndexList idxs=deselected.indexes();
  for (int i=0;i<idxs.size();i++)
    {
      QAbstractTreeItem *item=(QAbstractTreeItem*)idxs[i].internalPointer();
      ATrack *track=qobject_cast<ATrack*>(item->data());
      performTreeTrackDeselection(track);
    }
  
  // PERFORM SELECTION
  idxs=selected.indexes();
  
  bool multi = ((QApplication::keyboardModifiers() & (Qt::ShiftModifier | Qt::ControlModifier)) > 0);
  for (int i=0;i<idxs.size();i++)
    {
      QAbstractTreeItem *item=(QAbstractTreeItem*)idxs[i].internalPointer();
      ATrack *track=qobject_cast<ATrack*>(item->data());
      performTreeTrackSelection(track,multi);
    }

  // The combine button should be enabled, because there are tracks selected
  buttonCombineTracks->setEnabled(tableSelectedTracks->selectionModel()->selectedRows().size()>1);
}

void ALayerGUI::performTreeTrackSelection(ATrack *track,bool multi)
{
  // Perform a recursive selection if this is a combination..
  if(track->type()==ATrack::eCombination)
    {
      multi=true; // Set multi to true, because we want to select all track in a combination...
      ATrackCombination *combo=qobject_cast<ATrackCombination*>(track);
      for(int i=0;i<combo->size();i++)
	{
	  performTreeTrackSelection(combo->getTrack(i),multi);
	}
    }
  else
    {
      int id=track->trackID();
      geo->selectTrackByID(id,multi);
    }
}

void ALayerGUI::performTreeTrackDeselection(ATrack *track)
{
  // Perform a recursive deselection if this is a combination..
  if(track->type()==ATrack::eCombination)
    {
      ATrackCombination *combo=qobject_cast<ATrackCombination*>(track);
      for(int i=0;i<combo->size();i++)
	{
	  performTreeTrackDeselection(combo->getTrack(i));
	}
    }
  else
    {
      int id=track->trackID();
      geo->deselectTrackByID(id);
    }
}

void ALayerGUI::about()
{
  QLabel *icon = new QLabel;
  icon->setPixmap(QPixmap(":/media/AMELIA_logo.png"));

  QLabel *text = new QLabel;
  text->setWordWrap(true);
  text->setText(
		"<p><font color=red><b>AMELIA</b></font><br><b>ATLAS Multimedia Educational Lab for Interactive Analysis</b><br>v0.1 - Alpha rev260</p>"
		" <p>Released as open-source software under the BSD license."
		" For more information, please read license.txt</p>"

		"<p><font color=blue>Project Coordinator </font>"
		"- Michael Barnett</p>"

		"<p><font color=blue>Project Director </font> "
		"- Joao Pequenao</p>"

		"<p><font color=blue>Developers</font>"
		" - Joao Pequenao, Karol Krizka, Jacob Andreas, Brian Amadio</p>"

		"<p><font color=blue>Design and Interface consultants</font>"
		" - Paul Schaffner, Emily Greenberg</p>"

		"<p><font color=blue>Education Specialist</font>"
		" - Kris Whelan</p>"

		"<p><font color=blue>Additional Contributions by</font>"
		" - David Medovoy, Guil Miranda</p>"

		"<p><font color=blue>Portions of this code were inspired by</font>"
		" - Christian Stehno (aka hybrid), Travis Vitek</p>"

		"<p><font color=blue>Special thanks to</font>"
		" - Nikolaus Gebhardt, Gersende Prior, Sven Vahsen, Dugan O'Neil, The Irrlicht Forum</p>"
		);

  QPushButton *quitButton = new QPushButton("OK");

  QHBoxLayout *topLayout = new QHBoxLayout;
  topLayout->setMargin(10);
  topLayout->setSpacing(10);
  topLayout->addWidget(icon);
  topLayout->addWidget(text);

  QHBoxLayout *bottomLayout = new QHBoxLayout;
  bottomLayout->addStretch();
  bottomLayout->addWidget(quitButton);
  bottomLayout->addStretch();

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addLayout(topLayout);
  mainLayout->addLayout(bottomLayout);

  QDialog about(this);
  about.setModal(true);
  about.setWindowTitle(tr("About AMELIA"));
  about.setLayout(mainLayout);

  connect(quitButton, SIGNAL(clicked()), &about, SLOT(close()));

  about.exec();
}
