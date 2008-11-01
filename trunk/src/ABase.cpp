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

#include "AUILoader.h"
#include "QGraphicsClickablePixmapItem.h"
#include <QGLWidget>
ABase::ABase( QWidget *parent )
        : QMainWindow(parent)
{
    //Set to the initial size.
    setFixedSize(1024,768); //Makes the widget unresizable..

    layout.setContentsMargins(0,0,0,0);
    center.setLayout(&layout);

    //Some settings for the scene
    menu.setSceneRect(0,0,1024,768); //Aka disable scrolling
    menu.addItem(&widgetGroup);

    //Initialize the widget group that handles the animations
    widgetGroup.scale(0.25,0.25);
    widgetGroup.setPos(calculateScaledWidgetGroupPosition());
    widgetGroup.setHandlesChildEvents(false); //Let the pixmap handle it's own clicks

    //Make the widget without scrollbars and display it
    menuWidget.setScene(&menu);
    menuWidget.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    menuWidget.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setupViewport();
    menuWidget.setStyleSheet("border: none"); //Makes the border around the graphics view dissapear
    layout.addWidget(&menuWidget);

    setCentralWidget(&center);

    //This mapper will be used for connecting pixmaps to changeToLevel
    connect(&mapper,SIGNAL(mapped(const QString&)),
            this,SLOT(changeToLevel(const QString&)));


    //Used by transition animations
    timer.setDuration(500);
    animation.setItem(&widgetGroup);
    animation.setTimeLine(&timer);
    connect(&timer,SIGNAL(finished()),
            this,SLOT(animationFinished()));
}

ABase::~ABase() { }

void ABase::setupViewport()
{
    QWidget *viewport;

#ifdef Q_WS_WIN
    viewport=new QWidget(&menuWidget);
    viewport->setAttribute(Qt::WA_MSWindowsUseDirect3D);
#else
    QGLWidget *glw = new QGLWidget(QGLFormat(QGL::SampleBuffers));
    glw->setAutoFillBackground(false);
    viewport = glw;
#endif
    menuWidget.setCacheMode(QGraphicsView::CacheNone);
    menuWidget.setViewport(viewport);
}

void ABase::addLevel(QString uicfile)
{
    //Everything else should be an UI file (for now)
    AUILoader loader;
    QFile file(":/ui/"+uicfile);
    file.open(QFile::ReadOnly);
    QWidget *tmp = loader.load(&file,this);
    file.close();

    if (!tmp)
    {
        QMessageBox::critical(this,"Failed to change UI","There was a problem loading the UI named \""+uicfile+"\". This could be caused by a corrupt installation.");
        return;
    }

    //Make sure to disable the widget
    tmp->setEnabled(false);
    tmp->setFixedSize(1024,768);
    tmp->hide();

    //TODO Implement maximum number of columns!
    //int col=screens.size()%3;
    //int row=screens.size()/3;

    //HACK (ugly) setup the conenctions of the ALayerGUI
    //TODO figure out a more general way to do this
    if (uicfile=="geometry.ui")
        ((ALayerGUI*)((QMainWindow*)tmp)->centralWidget())->setupElements();

    if (uicfile == "wikibrowser.ui" || uicfile == "newsbrowser.ui")
    {
        //should setup the browsers to allow webpages bigger than 1024x768 when rendered. Maybe a scrollbar
    }

    //Add it to the menu item...
    QPixmap ss=QPixmap::grabWidget(tmp);
    QGraphicsClickablePixmapItem* item=new QGraphicsClickablePixmapItem(ss,&widgetGroup);
    item->setPos(1024*(widgets.size()-1.5),0);

    //Add the pixmap to the signal map, which will let us switch menus
    mapper.setMapping(item,uicfile);
    connect(item,SIGNAL(clicked()),
            &mapper,SLOT(map()));

    //Save it all to our fancy dancy maps
    items[uicfile]=item;
    widgets[uicfile]=tmp;
    layout.addWidget(tmp);

    //Update the positions of the itmers
    if (menuWidget.isVisible() && current.isEmpty())
        changeToMenu(); // Menu is currently displayed, so use an animation
    else
        widgetGroup.setPos(calculateScaledWidgetGroupPosition()); // Not displayed, so just move it directly
}

void ABase::changeToMenu()
{
    if (timer.state()==QTimeLine::Running) return; //TODO Allow changing levels in mid transition

    if (current.isEmpty()) return; //Already on menu..
    else
    {
        widgets[current]->setEnabled(false);
        QPixmap ss=QPixmap::grabWidget(widgets[current]);
        items[current]->setPixmap(ss);

        setUpdatesEnabled(false);
        widgets[current]->hide();
        menuWidget.show();
        setUpdatesEnabled(true);
    }

    //Initial condiftions
    if (current.isEmpty())
        animation.setScaleAt(0,0.25,0.25);
    else
        animation.setScaleAt(0,1,1);

    animation.setPosAt(0,widgetGroup.pos());

    // Scale of 0.2 is
    // widget width = 1024*0.2 = 204.8
    // widget height = 768*0.2 = 153.6
    animation.setScaleAt(1,0.25,0.25);
    animation.setPosAt(1,calculateScaledWidgetGroupPosition());

    timer.start();

    previous=current;
    current=QString();
}

void ABase::changeToLevel(const QString& uicfile)
{
    if (timer.state()==QTimeLine::Running) return; //TODO Allow changing levels in mid transition

    if (uicfile==current) return; //Arealdy there...
    else if (!current.isEmpty())
    {
        widgets[current]->setEnabled(false);
        QPixmap ss=QPixmap::grabWidget(widgets[current]);
        items[current]->setPixmap(ss);

        setUpdatesEnabled(false);
        widgets[current]->hide();
        menuWidget.show();
        setUpdatesEnabled(true);
    }

    //Initial conditions
    if (current.isEmpty())
        animation.setScaleAt(0,0.25,0.25);
    else
        animation.setScaleAt(0,1,1);

    animation.setPosAt(0,widgetGroup.pos());

    animation.setScaleAt(1,1,1);


    animation.setPosAt(1,-items[uicfile]->pos());

    timer.start();

    //For some reason setting previous to current here would make uicfile a "". Don't ask...
    QString tmp=current;
    current=uicfile;
    previous=tmp;
}

void ABase::animationFinished()
{
    if (current.isEmpty()) return;

    widgets[current]->setEnabled(true);

    setUpdatesEnabled(false);
    menuWidget.hide();
    widgets[current]->show();
    setUpdatesEnabled(true);
}

//TEST TEST Lets me quickly test the animations without having to add buttons TEST TEST
void ABase::keyPressEvent(QKeyEvent *event)
{
    if (event->key()==Qt::Key_Escape)
    {
        if (current.isEmpty() && !previous.isEmpty())
            changeToLevel(previous);
        else
            changeToMenu();
    }

    if (event->key()==Qt::Key_Delete)
    {
        QList<QString> keys=widgets.keys();
        int idx=keys.indexOf(current);
        idx=(idx+1)%keys.size();
        changeToLevel(keys[idx]);
    }
}

QPointF ABase::calculateScaledWidgetGroupPosition()
{
    //Figures out where the widget group would go if it were scaled by 0.2
    // and centered
    QPointF ret= QPointF((1024-widgetGroup.sceneBoundingRect().width())/2,
                         (768-widgetGroup.sceneBoundingRect().height())/2 - 200);

    return ret;
}

void ABase::updatePixmaps()
{
    QList<QString> keys=widgets.keys();
    for (int i=0;i<keys.size();i++)
    {
    }
}

void ABase::on_GeoButton_clicked()
{
    changeToLevel("geometry.ui");
}

void ABase::on_CompizButton_clicked()
{
    changeToLevel("compiz.ui");
}

void ABase::on_MenuButton_clicked()
{
    changeToLevel("wikibrowser.ui");
}

void ABase::on_BackButton_clicked()
{
    changeToLevel("wikibrowser.ui");
}

void ABase::on_QuitButton_clicked()
{
    close();
}

void ABase::on_MenuButton_activated()
{
    changeToMenu();
}

void ABase::on_BackButton_activated()
{
    changeToLevel("wikibrowser.ui");
}

void ABase::on_QuitButton_activated()
{
    close();
}
