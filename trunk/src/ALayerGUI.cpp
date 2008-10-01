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

}

void ALayerGUI::setupElements()
{
    qDebug() << "Setting up elements";

    //Find all required widgets...
    tableSelectedTracks = findChild<QTableView*>("selectedTracksTable");
    tableCombinedTracks = findChild<QTableView*>("combinedTracksTable");
    tableInterestingTracks = findChild<QTableView*>("interestingTracksTable");
    geo=findChild<AGeometry*>("AGeometry");
    QPushButton *buttonDeleteTracks = findChild<QPushButton*>("deleteTracks");
    QPushButton *buttonCombineTracks = findChild<QPushButton*>("combineTracks");
    QGraphicsView *trackInfoView=findChild<QGraphicsView *>("trackInfo");
    QGraphicsView *eventInfoView=findChild<QGraphicsView *>("eventInfo");
    QTableView *detailedSelectedTracksTable=findChild<QTableView *>("detailedSelectedTracksTable");
    QTableView *detailedCombinedTracksTable=findChild<QTableView *>("detailedCombinedTracksTable");
    QAction *actionFPS=window()->findChild<QAction *>("actionFPS");
    QAction *actionSphere=window()->findChild<QAction *>("actionSphere");
    QPushButton *nextEventButton=findChild<QPushButton *>("nextEventButton");
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
    connect(geo->XmlEvt,SIGNAL(eventUnloaded()),
            tracksModel,SLOT(clear()));
    connect(geo->XmlEvt,SIGNAL(eventUnloaded()),
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
        connect(buttonCombineTracks,SIGNAL(clicked()), //The combine button...
                tracksModel,SLOT(combineSelectedTracks()));
         connect(geo->XmlEvt,SIGNAL(eventLoaded(QString)),
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
        trackInfo=new AEventInfoScene;
        trackInfo->type = AEventInfoScene::infoTrack;
        trackInfo->init();
        trackInfoView->setScene(trackInfo);
        trackInfoView->ensureVisible(0,0,450,300,10,10);
        trackInfo->geo = geo;

        //Signals...
        connect(geo,SIGNAL(trackSelected(track*)), //track selection
                trackInfo,SLOT(updateTrackInfo(track*)));
        connect(geo,SIGNAL(trackDeselected(track*)), //track selection
                trackInfo,SLOT(removeTrackInfo(track*)));

        connect(trackInfo,SIGNAL(sig_addTrackToTable(track*)), //add to table button, add to table
                tracksModel,SLOT(addTrack(track*)));

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
        eventInfo->type = AEventInfoScene::infoEvent;
        eventInfo->init();
        eventInfoView->setScene(eventInfo);
        eventInfoView->ensureVisible(0,0,450,300,10,10);
        eventInfo->geo = geo;

        connect(geo->XmlEvt,SIGNAL(eventChanged()),
                eventInfo,SLOT(updateEventInfo()));
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
        connect(geo->XmlEvt,SIGNAL(eventLoaded(QString)),
                this,SLOT(handleEventLoaded()));
	connect(geo->XmlEvt,SIGNAL(eventUnloaded()),
		this,SLOT(handleEventUnloaded()));


        signalMapper=new QSignalMapper(this);
        signalMapper->setMapping(actionSphere, AGeometry::Maya);
        signalMapper->setMapping(actionFPS, AGeometry::FPS);

	connect(actionSphere,SIGNAL(triggered()),
		signalMapper, SLOT(map()));
	connect(actionFPS,SIGNAL(triggered()),
		signalMapper, SLOT(map()));
	connect(signalMapper, SIGNAL(mapped(int)),
		geo, SLOT(setCamera(int)));
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
    geo->XmlEvt->Event = geo->XmlEvt->DisplayParticles(geo->XmlEvt->P_checkbox_states, geo->XmlEvt->Event);
}


void ALayerGUI::ptCutoff(int cutoff)
{
    geo->XmlEvt->ptcut = cutoff/1000;
    geo->XmlEvt->PtCutoff(geo->XmlEvt->ptcut, geo->XmlEvt->EventComplete);

}

void ALayerGUI::showLoadEventDialog()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Event"), "./media/events/", tr("Event Files (*.xml)"));

    loadEvent (fileName);
}

void ALayerGUI::loadNextEvent()
{
    QDir directory("./media/events");
    QStringList filters;
    filters << "*.xml";
    QStringList events=directory.entryList(filters);
    int eventID=qrand()%events.size();
    QString file=events.at(eventID);
    QString pathToFile=directory.filePath(file);
    loadEvent(pathToFile);
}

bool ALayerGUI::loadEvent(QString fileName)
{
    cout << "Loading " << fileName.toAscii().data() << endl;

    //TODO Error somehow
    if (!geo) return false;

    if (!fileName.isEmpty())
    {
      IFileSystem *fs=geo->GetDevice()->getFileSystem();

      if (!fs->existFile(fileName.toAscii().data()))
        {
            QMessageBox::warning(this, tr("Application"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(fileName));
            return false;
        }

        QApplication::setOverrideCursor(Qt::WaitCursor);

        //char* charfile = new char[200];
        //strcpy ( charfile, fileName.toAscii().data() );

        geo->XmlEvt->LoadEvent(fileName.toAscii().data());

		tourBuilder->markLoadEvent (cstr_to_wstr(fileName.toAscii().data(),fileName.length()));
    }
	return true;
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
    if (actionTable) actionTable->setEnabled(true);

    QApplication::restoreOverrideCursor();

    show(eventWidget);
}

void ALayerGUI::handleEventUnloaded()
{

}

void ALayerGUI::show(QWidget *w)
{
    if (!w) return;
    w->show();

    AAnimationGUI *anim=new AAnimationGUI(w);
    anim->setKeyframe(100,widgetPositions[w->objectName()]);
    connect(anim,SIGNAL(animationFinished()),
            anim,SLOT(deleteLater()));
    anim->play();
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
    if(dev->getFileSystem()->existFile("media/tours"))
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

    QObject::connect (geo, SIGNAL(repainted()), tourManager, SLOT(onUpdate()));

    QObject::connect (tourManager, SIGNAL(tour_camera(AGeometry::CameraMode)), geo, SLOT(setViewport(AGeometry::CameraMode)));
    QObject::connect (tourManager, SIGNAL(tour_loadfile(QString)), this, SLOT(loadEvent(QString)));
    QObject::connect (tourManager, SIGNAL(tour_stopped()), this, SLOT(endTour()));
    QObject::connect (tourManager, SIGNAL(tour_button(char *)), this, SLOT(pressButton(char *)));
    QObject::connect (tourManager, SIGNAL(tour_ptchange(int)), this, SLOT(ptCutoff(int)));

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

