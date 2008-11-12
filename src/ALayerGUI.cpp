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

#include "ALayerGUI.h"

#include "AAnimationGUI.h"

#include <config.h>

ALayerGUI::ALayerGUI(QWidget* parent)
        : QFrame(parent)
{
    directoryBase = new char[100]; // MAGIC NUMBER
    QString cwd=QDir::currentPath();
    strcpy ( directoryBase, cwd.toAscii().data() );

    tourBuilder = 0;
    tourManager = 0;
}

ALayerGUI::~ALayerGUI()
{
    delete mngr;
}

void ALayerGUI::setupElements()
{
    qDebug() << "Setting up elements";

    //Find all required widgets...
    tableSelectedTracks = findChild<QTableView*>("selectedTracksTable");
    tableCombinedTracks = findChild<QTableView*>("combinedTracksTable");
    tableInterestingTracks = findChild<QTableView*>("interestingTracksTable");
    geo=findChild<AGeometry*>("AGeometry");
    menuTagCurrentEvent=window()->findChild<QMenu *>("menuTagCurrentEvent");
    buttonDeleteTracks = findChild<QPushButton*>("deleteTracks");
    buttonCombineTracks = findChild<QPushButton*>("combineTracks");
    QGraphicsView *trackInfoView=findChild<QGraphicsView *>("trackInfo");
    QGraphicsView *eventInfoView=findChild<QGraphicsView *>("eventInfo");
    QTableView *detailedSelectedTracksTable=findChild<QTableView *>("detailedSelectedTracksTable");
    QTableView *detailedCombinedTracksTable=findChild<QTableView *>("detailedCombinedTracksTable");
    QAction *actionFPS=window()->findChild<QAction *>("actionFPS");
    QAction *actionSphere=window()->findChild<QAction *>("actionSphere");
    packageList = findChild<QTreeView*>("packageList");
    QPushButton *nextEventButton = findChild<QPushButton*>("nextEventButton");
    PtCutoff_Slider=findChild<QSlider*>("PtCutoff_Slider");
    actionTagHiggsBoson=window()->findChild<QAction *>("actionTagHiggsBoson");
    actionTagBlackHole=window()->findChild<QAction *>("actionTagBlackHole");
    selectedEventInfoView=findChild<QGraphicsView *>("selectedEventInfo");

    geo->actFPS = actionFPS;
    geo->actSphere = actionSphere;

    eventWidget=findChild<QWidget *>("eventWidget");
    AGeometryFrame=findChild<AMainView *>("AGeometryFrame");

    tracksModel=new ATrackTableModel(this);
    interestingTracksModel=new AInterestingTrackTableModel(this);
    interestingTracksModel->geo = geo;
    comboModel=new AComboTableModel(tableCombinedTracks);
    connect(tracksModel,SIGNAL(tracksCombined(ATrackCombination*)),
            comboModel,SLOT(addCombination(ATrackCombination*)));
    connect(this,SIGNAL(eventUnloaded()),
            tracksModel,SLOT(clear()));
    connect(this,SIGNAL(eventUnloaded()),
            comboModel,SLOT(clear()));

    //Setup Tables
    if (tableSelectedTracks)
    {
        tracksModel->addTable(tableSelectedTracks);

        tableSelectedTracks->setColumnWidth ( 0, 60 );
        tableSelectedTracks->setColumnWidth ( 1, 60 );
        tableSelectedTracks->setColumnWidth ( 2, 70 );

        connect(tracksModel,SIGNAL(entrySelected(int,bool)),
                geo,SLOT(selectTrackByID(int,bool)));
        connect(tracksModel,SIGNAL(entryDeselected(int)),
                geo,SLOT(deselectTrackByID(int)));
        connect(buttonCombineTracks,SIGNAL(clicked()), //The combine button...
                tracksModel,SLOT(combineSelectedTracks()));

    }

    if (tableInterestingTracks)
    {
        interestingTracksModel->addTable(tableInterestingTracks);

        tableInterestingTracks->setColumnWidth ( 0, 60 );
        tableInterestingTracks->setColumnWidth ( 1, 60 );
        tableInterestingTracks->setColumnWidth ( 2, 70 );

        connect(interestingTracksModel,SIGNAL(entrySelected(int,bool)),
                geo,SLOT(selectTrackByID(int,bool)));
        connect(interestingTracksModel,SIGNAL(entryDeselected(int)),
                geo,SLOT(deselectTrackByID(int)));
        connect(this,SIGNAL(eventLoaded(QString)),
                interestingTracksModel,SLOT(getInterestingTracks()));
    }

    if (tableCombinedTracks)
    {
        comboModel->addTable(tableCombinedTracks);

        //TODO: Can't we set this in designer?
        tableCombinedTracks->setColumnWidth ( 0, 90 );
        tableCombinedTracks->setColumnWidth ( 1, 50 );
        tableCombinedTracks->setColumnWidth ( 2, 70 );
    }

    if (detailedCombinedTracksTable)
        comboModel->addTable(detailedCombinedTracksTable);
    if (detailedSelectedTracksTable)
        tracksModel->addTable(detailedSelectedTracksTable);


    // Setup trackview
    if (trackInfoView)
    {
        trackInfo=new ASelectionInfoScene;
        trackInfoView->setScene(trackInfo);
        trackInfoView->ensureVisible(0,0,450,300,10,10);

        //Signals...
        connect(geo,SIGNAL(trackSelected(ATrack*)), //track selection
                trackInfo,SLOT(updateTrackInfo(ATrack*)));
        connect(geo,SIGNAL(trackDeselected(ATrack*)), //track selection
                trackInfo,SLOT(removeTrackInfo(ATrack*)));

        connect(trackInfo,SIGNAL(sig_addTrackToTable(ATrack*)), //add to table button, add to table
                tracksModel,SLOT(addTrack(ATrack*)));

        connect(trackInfo,SIGNAL(sig_addComboToTable(ATrackCombination*)), //add to table button, add to table
                comboModel,SLOT(addCombination(ATrackCombination*)));

        connect(geo,SIGNAL(viewportSwitched(int,int)),//Basically clear the track selection on camera change
                trackInfo,SLOT(handleViewportChange(int,int)));

        connect(trackInfo,SIGNAL(combineButtonEnabled(bool)),
                buttonCombineTracks,SLOT(setEnabled(bool)));
    }


    // Setup event view
    if (eventInfoView)
    {
        eventInfo=new AEventInfoScene;
        eventInfo->setEvent(geo->XmlEvt);
        eventInfoView->setScene(eventInfo);
        eventInfoView->ensureVisible(0,0,450,300,10,10);

        connect(geo->XmlEvt,SIGNAL(eventChanged()),
                eventInfo,SLOT(updateEventInfo()));
    }

    // Setup package list

    QDir ws("amelia:workspace");
    mngr=new AEventManager(ws.absolutePath());
    if (packageList)
    {
        packageList->setModel(mngr);
		//This crashes QPixmap::grabWidget() when the list is not visible for some reason...
        packageList->setColumnWidth(0,200);
        connect(packageList, SIGNAL(doubleClicked ( const QModelIndex& )),
                this, SLOT(loadEventFromManager(const QModelIndex& )));
        connect(this,SIGNAL(eventLoaded(QString)),
                mngr, SLOT(eventLoaded(QString)));
        connect(this,SIGNAL(eventUnloaded()),
                mngr, SLOT(eventUnloaded()));

        //The selected event info is disabled right now...
        //connect(packageList, SIGNAL(clicked( const QModelIndex& )),
        //this, SLOT(updateEventTagInfo(const QModelIndex& )));
    }

    // Setup random buttons
    if (buttonDeleteTracks)
    {
        connect(buttonDeleteTracks,SIGNAL(clicked()),
                tracksModel,SLOT(deleteSelectedTracks()));
    }
    if (nextEventButton)
    {
        connect(nextEventButton,SIGNAL(clicked()),
                this,SLOT(loadNextEvent()));
    }
    if (PtCutoff_Slider)
        connect(PtCutoff_Slider,SIGNAL(valueChanged(int)),
                geo->XmlEvt,SLOT(PtCutoff(int)));


    //setup tour button connections

    QList <QAbstractButton *> buttons = findChildren <QAbstractButton *> ();

    while (!buttons.isEmpty ())
    {

        QAbstractButton *b  = buttons.takeFirst ();

        QString name = b->objectName ();

        cout << name.toAscii ().data() << endl;

        if (name == "") continue;
        if (name == "recTourButton" || name == "snapshotTourButton" || name == "playTourButton" || name == "ffTourButton") continue;
        QObject::connect (b, SIGNAL(clicked()), this, SLOT(recordButtonPress()));

    }

    if (eventWidget)
    {
        //Must remove widget from layout, otherwise it flashes even if WA_Moved is set...
        findChild<QLayout *>("eventWidgetLayout")->removeWidget(eventWidget);
        eventWidget->hide();
    }

    if (geo) //Setup signals pertaining to AGeometry
    {
        signalMapper=new QSignalMapper(this);
        signalMapper->setMapping(actionSphere, AGeometry::Maya);
        signalMapper->setMapping(actionFPS, AGeometry::FPS);

        connect(actionSphere,SIGNAL(triggered()),
                signalMapper, SLOT(map()));
        connect(actionFPS,SIGNAL(triggered()),
                signalMapper, SLOT(map()));
        connect(signalMapper, SIGNAL(mapped(int)),
                geo, SLOT(setCamera(int)));
        connect(actionTagBlackHole,SIGNAL(toggled(bool)),
                this, SLOT(handleEventTagChange(bool)));
        connect(actionTagHiggsBoson,SIGNAL(toggled(bool)),
                this, SLOT(handleEventTagChange(bool)));
    }

    selectedEventInfo=new AEventInfoScene;
    if (selectedEventInfoView)
    {
        selectedEventInfoView->setScene(selectedEventInfo);
        selectedEventInfoView->ensureVisible(0,0,450,300,10,10);
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
    else if (from=="actionOrthogonal")
    {
        geo->setViewport(AGeometry::Orthogonal);
    }
    else if (from=="actionProjective")
    {
        geo->setViewport(AGeometry::Projective);
    }
}

//slots for event handling

void ALayerGUI::selectJetModel(QString jetType)
{
    geo->XmlEvt->setCurrentJetModel(jetType);
}

void ALayerGUI::selectTrackModel(QString trackType)
{
    geo->XmlEvt->setCurrentTrackModel(trackType);
}

void ALayerGUI::selectMisEtModel(QString metType)
{
    geo->XmlEvt->setCurrentMisEtModel(metType);
}


void ALayerGUI::toggleVisibilityParticles(bool toggle)
{

    QString from=sender()->objectName();

    if (from=="checkBox_Electrons")
    {
        geo->XmlEvt->P_checkbox_states[0] = toggle;
    }
    else if (from=="checkBox_Muons")
    {
        geo->XmlEvt->P_checkbox_states[1] = toggle;
    }
    else if (from=="checkBox_ChargedHadrons")
    {
        geo->XmlEvt->P_checkbox_states[5] = toggle;
    }
    else if (from=="checkBox_NeutralHadrons")
    {
        geo->XmlEvt->P_checkbox_states[4] = toggle;
    }
    else if (from=="checkBox_Photons")
    {
        geo->XmlEvt->P_checkbox_states[2] = toggle;
    }
    else if (from=="checkBox_Jets")
    {
        geo->XmlEvt->P_checkbox_states[6] = toggle;
    }
    else if (from=="checkBox_MissingEt")
    {
        geo->XmlEvt->P_checkbox_states[7] = toggle;
    }
    geo->XmlEvt->Event = geo->XmlEvt->DisplayParticles();
}


void ALayerGUI::showLoadEventDialog()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Event"), "./media/events/", tr("Event Files (*.xml)"));

    loadEvent (fileName);
}

void ALayerGUI::loadNextEvent()
{
    QModelIndex e=mngr->nextEvent();
    loadEventFromManager(e);
}

bool ALayerGUI::loadEvent(QString fileName)
{
    qDebug() << "Loading " << fileName << endl;

    //TODO Error somehow
    if (!geo) return false;

    if (!fileName.isEmpty())
    {
      QFile eventFile(fileName);

      if (!eventFile.exists())
        {
            QMessageBox::warning(this, tr("Application"),
                                 tr("Cannot find event file: %1!\n.")
                                 .arg(fileName));
            return false;
        }

        QApplication::setOverrideCursor(Qt::WaitCursor);

        if (geo->XmlEvt->EventComplete.Tracks.size()>0)
        {
            geo->XmlEvt->UnloadEvent();
            emit eventUnloaded();
        }

        geo->XmlEvt->LoadEvent(fileName);
        geo->XmlEvt->DisplayEvent(geo);
        geo->XmlEvt->PtCutoff(PtCutoff_Slider->sliderPosition());
        geo->XmlEvt->DisplayParticles();

        tourBuilder->markLoadEvent (cstr_to_wstr(fileName.toAscii().data(),fileName.length()));

        handleEventLoaded();
        emit eventLoaded(fileName);
    }
    return true;
}

void ALayerGUI::loadEventFromManager(const QModelIndex& index)
{
    if (!index.isValid()) return;

    AEvent *e=mngr->getEvent(index);
    if (e)
    {
        if (loadEvent(e->location))
        {
            menuTagCurrentEvent->setEnabled(true);
            actionTagHiggsBoson->setChecked(e->tags.contains("higgs"));
            actionTagBlackHole->setChecked(e->tags.contains("blackhole"));
        }
    }
    else
    {
        mngr->setActivePackage(index);

        int count=mngr->rowCount(QModelIndex());
        for (int i=0;i<count;i++)
        {
            QModelIndex idx=mngr->index(i,0,QModelIndex());
            if (i==index.row())
                packageList->expand(idx);
            else
                packageList->collapse(idx);
        }
    }
}

void ALayerGUI::handleEventLoaded()
{
    if (!eventWidget->isVisible())
    { // Move the eventWidget to some far far away place, if this is the first time an even was loaded
        widgetPositions["eventWidget"]=AGeometryFrame->geometry().bottomLeft();
        eventWidget->move(-AGeometryFrame->geometry().width()-10,widgetPositions["eventWidget"].y());
        eventWidget->setAttribute(Qt::WA_Moved);
    }

    geo->allowTrackSelection = true;
    QWidget* tabEvent = findChild<QWidget*>("tab_event");
    findChild<QTabWidget*>("MainTab")->setCurrentWidget(tabEvent);

    //Enable what needs to be enabled!
    QTableView *detailedSelectedTracksTable=findChild<QTableView *>("detailedSelectedTracksTable");
    QTableView *detailedCombinedTracksTable=findChild<QTableView *>("detailedCombinedTracksTable");
    QAction *actionTable=window()->findChild<QAction *>("actionTable");

    if (tabEvent) tabEvent->setEnabled(true);
    if (detailedSelectedTracksTable) detailedSelectedTracksTable->setEnabled(true);
    if (detailedCombinedTracksTable) detailedCombinedTracksTable->setEnabled(true);
    if (buttonDeleteTracks) buttonDeleteTracks->setEnabled(true);
    if (buttonCombineTracks) buttonCombineTracks->setEnabled(true);
    if (actionTable) actionTable->setEnabled(true);

    QApplication::restoreOverrideCursor();

    show(eventWidget);
}

void ALayerGUI::handleEventUnloaded()
{
    menuTagCurrentEvent->setEnabled(false);
}

void ALayerGUI::eventSettings()
{
    AUILoader loader;
    QFile file(":/ui/eventadvanced.ui");
    file.open(QFile::ReadOnly);
    QWidget *content = loader.load(&file,this);
    file.close();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(content);
    QDialog settings(0);
    settings.setWindowTitle(tr("Event Settings"));
    settings.setLayout(layout);

    QDialogButtonBox* buttonBox = content->findChild<QDialogButtonBox*>("buttonBox");

    AAdvancedEventSettings* evSettings = static_cast<AAdvancedEventSettings*>(content);

    connect(evSettings, SIGNAL(changedJetType(QString)), this, SLOT(selectJetModel(QString)));
    connect(evSettings, SIGNAL(changedTrackType(QString)), this, SLOT(selectTrackModel(QString)));
    connect(evSettings, SIGNAL(changedMisEtType(QString)), this, SLOT(selectMisEtModel(QString)));
    connect(evSettings, SIGNAL(redraw()), geo->XmlEvt,SLOT(DisplayParticles()));
    connect(evSettings, SIGNAL(redraw()), interestingTracksModel,SLOT(getInterestingTracks()));

    connect(buttonBox, SIGNAL(rejected()), &settings, SLOT(close()));
    connect(buttonBox, SIGNAL(accepted()), &settings, SLOT(close()));


    settings.exec();
}

void ALayerGUI::updateEventTagInfo(const QModelIndex& index)
{
    AEvent *e=mngr->getEvent(index);
    bool tagHiggs=false;
    bool tagBlack=false;
    bool enable=false;

    AXmlEvent *oldEvent=selectedEventInfo->event();
    if (oldEvent)
        oldEvent->deleteLater();

    if (e)
    {
        AXmlEvent *event=new AXmlEvent();
        if (PtCutoff_Slider)
            connect(PtCutoff_Slider,SIGNAL(valueChanged(int)),
                    event,SLOT(PtCutoff(int)));

        event->LoadEvent(e->location.toLocal8Bit().data());
        selectedEventInfo->setEvent(event);
        tagHiggs=e->tags.contains("higgs");
        tagBlack=e->tags.contains("blackhole");
        enable=true;
    }


}

void ALayerGUI::show(QWidget *w)
{
    if (!w) return;
    w->show();

    AAnimationGUI *anim=new AAnimationGUI(w);
    anim->setKeyframe(1000,widgetPositions[w->objectName()]);
    connect(anim,SIGNAL(animationFinished()),
            anim,SLOT(deleteLater()));
    anim->play();
}

void ALayerGUI::handleEventTagChange(bool status)
{
    QAction *from=(QAction*)sender();
    QString tag;
    if (from==actionTagHiggsBoson)
        tag="higgs";
    else
        tag="blackhole";

    if (status)
        mngr->tagActiveEvent(tag);
    else
        mngr->detagActiveEvent(tag);
}

void ALayerGUI::toggleTour ()
{

    if (tourManager->isRunning ()) endTour ();
    else startTour ();

}

void ALayerGUI::endTour ()
{

    tourManager->stop ();
    findChild<QPushButton *>("playTourButton")->setText ("Play");
    findChild<QPushButton *>("ffTourButton")->setEnabled (false);

}

void ALayerGUI::startTour ()
{

    findChild<QPushButton *> ("playTourButton")->setText ("Stop");
    findChild <QPushButton *> ("ffTourButton")->setEnabled (true);

    int selIndex = findChild<QComboBox *> ("tourSelector")->currentIndex ();

    tourManager->load (tourManager->tourFiles [selIndex]);

    //Make sure we use the correct camera.
    geo->setViewport(AGeometry::Cam3D);
    geo->setCamera(AGeometry::FPS,false);
    tourManager->begin ();

}


void ALayerGUI::ffTour ()
{

    if (tourManager->isRunning ()) tourManager->skip ();

}



void ALayerGUI::toggleRecording ()
{

    if (!tourBuilder->isRecording ())
    {

        tourBuilder->startRecording ("rectest.xml");
        findChild<QPushButton *> ("recTourButton")->setText ("Stop");
        findChild <QPushButton *> ("snapshotTourButton")->setEnabled (true);

    }
    else
    {

        tourBuilder->stopRecording ();
        findChild<QPushButton *> ("recTourButton")->setText ("Record");
        findChild <QPushButton *> ("snapshotTourButton")->setEnabled (false);

    }

}

void ALayerGUI::snapshotRecording ()
{

    tourBuilder->markCamera ();

}

void ALayerGUI::recordButtonPress ()
{

    wchar_t* name = cstr_to_wstr (QObject::sender ()->objectName ().toAscii ().data(), 100);

    tourBuilder->markAction (ATour::AT_BUTTON, name);

}

void ALayerGUI::prepareTours ()
{

    // If we are keeping strict MVC this should be handled
    // through a signal to the base app.
    // This will work for the time being

    cout << "preptours called" << endl;

    QComboBox* combo = parent()->findChild<QComboBox*> ("tourSelector");

    IrrlichtDevice *dev = geo->GetDevice ();

    tourManager = new ATourManager (dev->getFileSystem (), dev->getTimer ());
    tourBuilder = new ATourBuilder (dev->getFileSystem (), geo->camera [0]);

    dev->getFileSystem()->addFolderFileArchive ( dev->getFileSystem()->getWorkingDirectory() );
    if (dev->getFileSystem()->existFile("media/tours"))
        tourManager->listTours ("media/tours");
    else
        tourManager->listTours (TOURS_PREFIX);

    if (combo)
    {
        for (int i = 0; i < tourManager->numTours; i++)
        {

            if (strlen (tourManager->tourNames[i]) != 0)
                combo->addItem (tourManager->tourNames[i]);

        }
    }

    QObject::connect (tourManager, SIGNAL(camAnimatorAdded(irr::core::array<vector3df>)), geo, SLOT(addCamAnimator(irr::core::array<vector3df>)));
    QObject::connect (tourManager, SIGNAL(tarAnimatorAdded(irr::core::array<vector3df>)), geo, SLOT(addTarAnimator(irr::core::array<vector3df>)));

    QObject::connect (tourManager, SIGNAL(camAnimatorRemoved()), geo, SLOT(removeCamAnimator ()));
    QObject::connect (tourManager, SIGNAL(tarAnimatorRemoved()), geo, SLOT(removeTarAnimator ()));

    QObject::connect (tourManager, SIGNAL(tour_camera(AGeometry::CameraMode)), geo, SLOT(setViewport(AGeometry::CameraMode)));
    QObject::connect (tourManager, SIGNAL(tour_loadfile(QString)), this, SLOT(loadEvent(QString)));
    QObject::connect (tourManager, SIGNAL(tour_stopped()), this, SLOT(endTour()));
    QObject::connect (tourManager, SIGNAL(tour_button(char *)), this, SLOT(pressButton(char *)));
    QObject::connect (tourManager, SIGNAL(tour_ptchange(int)), geo->XmlEvt, SLOT(PtCutoff(int)));

	geo->setCamera(AGeometry::Maya);
    //QCoreApplication::installEventFilter (this);

}

void ALayerGUI::fakeCursor (int x, int y)
{

    cout << "faking " << x << " " << y << endl;

    QMouseEvent *evt = new QMouseEvent (QEvent::MouseButtonPress, QPoint (x, y), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);

    QCoreApplication::postEvent (parent (), evt);

    evt = new QMouseEvent (QEvent::MouseButtonRelease, QPoint (x, y), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);

    QCoreApplication::postEvent (parent (), evt);

    QCoreApplication::flush ();
    QCoreApplication::processEvents ();

    cout << "done" << endl;


}

void ALayerGUI::pressButton (char* name)
{

    findChild <QAbstractButton *> (name)->click ();

}

void ALayerGUI::about()
{
    QLabel *icon = new QLabel;
    icon->setPixmap(QPixmap(":/media/AMELIA_logo.png"));

    QLabel *text = new QLabel;
    text->setWordWrap(true);
    text->setText(
        "<p><font color=red><b>AMELIA</font><br>ATLAS Multimedia Educational Lab for Interactive Analysis</b></p>"
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


