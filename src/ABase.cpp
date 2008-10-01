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

#include "ABase.h"

#include "AMenu.h"
#include "AGeometry.h"
#include "AUILoader.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

ABase::ABase( QWidget *parent )
        : QMainWindow(parent),widget(0)
{
    //Set to the initial size.
    resize(1024,730);

    //Create a widget to act as the central widget. Any UI will be layed
    //out into this widget.
    //We cannot use directly the central widget, because QMainWindow
    //switches between them improperly. It deletes the old one using the
    //delete instead of deleteLater(). This crashes Irrlicht's event handler.
    setCentralWidget(new QWidget(this));
    layout=new QVBoxLayout(this->centralWidget());
    layout->setContentsMargins(0,0,0,0); //Set margins to 0 everywhere
}

ABase::~ABase() { }

void ABase::changeToLevel(QString uicfile)
{

    //Get rid of last UI.
    if (widget!=0)
    {
        //Something already loaded. Clear it!
        layout->removeWidget(widget);

        setStyleSheet("");

        setMenuBar(0);
        setStatusBar(0);
        QList<QToolBar *> toolbars=findChildren<QToolBar *>();
        for (int i=0;i<toolbars.size();i++)
        {
            removeToolBar(toolbars[i]);
            toolbars[i]->deleteLater();
        }


        widget->deleteLater();
    }

    //Menu has special loading. Just make the central widget of the main window an AMenu.
    if (uicfile=="menu")
    {
        widget=new AMenu(this->centralWidget());
        //Connect load requests to this thing.
        QObject::connect(widget, SIGNAL(changeToLevel(QString)),
                         this, SLOT(changeToLevel(QString)));
    }
    else
    {
        //Everything else should be an UI file (for now)
        AUILoader loader;
        QFile file(":/ui/"+uicfile);
        file.open(QFile::ReadOnly);
        tmp = loader.load(&file,this);
        file.close();
        if (!tmp)
        {
            QMessageBox::critical(this,"Failed to change UI","There was a problem loading the UI named \""+uicfile+"\". This could be caused by a corrupt installation.");
        }
        else if (tmp->inherits("QMainWindow"))  //This is a Main Window, copy
        {
            //central widgets and everything!
            widget=((QMainWindow*)tmp)->centralWidget();


            setMenuBar(((QMainWindow*)tmp)->menuBar());
            //TODO Figure out how to remove a status bar from the designer file
            // Currently, even if we don't have a status bar one is created...
            // So for now, status bars have been disabled from all of Amelia!!
            //setStatusBar(((QMainWindow*)tmp)->statusBar());

            //Setup the toolbars of the main window
            QList<QToolBar *> toolbars=((QMainWindow*)tmp)->findChildren<QToolBar *>();
            for (int i=0;i<toolbars.size();i++)
            {
                QToolBar* bar=toolbars[i];
                bar->setParent(this);
                addToolBar(bar);
            }

            //Note: We do not try to delete the old QMainWindow, because
            //doing so will remove menu actions too.
            //

            /*QObject::connect (widget->findChild <AGeometry*> (), SIGNAL(finishedLoading()), widget, SLOT(prepareTours()));
            QObject::connect (widget->findChild <QPushButton*> ("playTourButton"), SIGNAL(clicked()), widget, SLOT(toggleTour()));
            QObject::connect (widget->findChild <QPushButton*> ("ffTourButton"), SIGNAL(clicked()), widget, SLOT(ffTour()));
            QObject::connect (widget->findChild <QPushButton*> ("recTourButton"), SIGNAL(clicked()), widget, SLOT(toggleRecording ()));
            QObject::connect (widget->findChild <QPushButton*> ("snapshotTourButton"), SIGNAL(clicked()), widget, SLOT(snapshotRecording ()));*/

            if (uicfile=="geometry.ui")
            {
                ALayerGUI *layer = (ALayerGUI *)widget;
                layer->setupElements();
                //QObject::connect (layer->tourManager, SIGNAL(camAnimatorAdded(irr::core::array<vector3df>)), layer->findChild <AGeometry*> (), SLOT(addCamAnimator(irr::core::array<vector3df>)));
                //
            }


        }
        else
        {
            //Classic QWidget. Just set it to center.
            widget=tmp;
        }
        widget->setParent(centralWidget());

        setStyleSheet(tmp->styleSheet());

    }

    //Connect everything else.
    QMetaObject::connectSlotsByName(this);

    //Grab the keyboard. This is needed for sending global shortcuts to AGeometry.
    widget->setFocus(Qt::OtherFocusReason);

    //Add the widget
    layout->addWidget(widget);
}

void ABase::on_GeoButton_clicked()
{
    changeToLevel("geometry.ui");
}

void ABase::on_MenuButton_clicked()
{
    changeToLevel("menu");
}

void ABase::on_BackButton_clicked()
{
    changeToLevel("menu.ui");
}

void ABase::on_QuitButton_clicked()
{
    close();
}

void ABase::on_MenuButton_activated()
{
    changeToLevel("menu");
}

void ABase::on_BackButton_activated()
{
    changeToLevel("menu.ui");
}

void ABase::on_QuitButton_activated()
{
    close();
}
