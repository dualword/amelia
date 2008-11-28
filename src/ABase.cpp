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
#include <QGraphicsSceneMouseEvent>

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
    menu.installEventFilter(this);

    //Initialize the background
    background=menu.addPixmap(QPixmap(":/media/CommandersBridge.png"));
    background->setZValue(0);
    background->setPos(calculateBackgroundPosition());

    //Timers for restoring parallax animations
    parallaxTimer.setDuration(1000);
    parallaxTimer.setCurveShape(QTimeLine::EaseInOutCurve);
    parallaxAnimation.setItem(&widgetGroup);
    parallaxAnimation.setTimeLine(&parallaxTimer);
    parallaxTimerBg.setDuration(1000);
    parallaxTimerBg.setCurveShape(QTimeLine::EaseInOutCurve);
    parallaxAnimationBg.setItem(background);
    parallaxAnimationBg.setTimeLine(&parallaxTimerBg);

    //Initialize the widget group that handles the animations
    widgetGroup.scale(0.25,0.25);
    widgetGroup.setPos(calculateScaledWidgetGroupPosition());
    widgetGroup.setHandlesChildEvents(false); //Let the pixmap handle it's own clicks
    widgetGroup.setZValue(100);

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

    connect(&updateMapper,SIGNAL(mapped(const QString)),
            this,SLOT(updatePixmap(const QString)));


    //Used by transition animations
    timer.setDuration(1000);
    animation.setItem(&widgetGroup);
    animation.setTimeLine(&timer);
    connect(&timer,SIGNAL(finished()),
            this,SLOT(animationFinished()));
}

ABase::~ABase() { }

void ABase::addPixmapUpdateReason(QString objectClass,const char *signal)
{
    QList<const char*> currentList;
    currentList=reasonsToUpdate[objectClass];
    currentList.push_back(signal);
    reasonsToUpdate[objectClass]=currentList;
}

void ABase::setupViewport()
{
    QWidget *viewport;

#ifdef Q_WS_WIN
    viewport=new QWidget(&menuWidget);
    viewport->setAttribute(Qt::WA_MSWindowsUseDirect3D);
    menuWidget.setCacheMode(QGraphicsView::CacheNone);
#else
    //Disabled OpenGL acceleration due to problems with Irrlicht
    //QGLWidget *glw = new QGLWidget(QGLFormat(QGL::SampleBuffers));
    //glw->setAutoFillBackground(false);
    viewport = new QWidget(&menuWidget);
    menuWidget.setCacheMode(QGraphicsView::CacheBackground);
    menuWidget.setOptimizationFlag(QGraphicsView::DontClipPainter);
    background->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
#endif
    menuWidget.setViewport(viewport);
    menuWidget.setRenderHint( QPainter::Antialiasing );
}

void ABase::addLevel(QString uicfile,QString description)
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

    //Add it to the menu item...
    QPixmap ss=QPixmap::grabWidget(tmp);
    QGraphicsClickablePixmapItem* item=new QGraphicsClickablePixmapItem(ss,&widgetGroup);
    item->setPos(1024*(widgets.size()-1.5),0);
    item->setAcceptHoverEvents(true); //For some reason doing this in the constructor of the pixmap item has no effect.

#ifdef Q_WS_X11
    item->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
#endif

    //Add a thing...

    QFont font;
    font.setBold( true );
    font.setPointSize( 24 );
    font.setStyleHint( QFont::Times );
    font.setStyleStrategy( QFont::PreferAntialias );


    QGraphicsSimpleTextItem *descItem=menu.addSimpleText(description);
    descItem->setBrush(QColor("white"));
    descItem->setFont(font);
    descItem->setPos(QPointF(-descItem->boundingRect().width(),0));

    //Rotate it. We're using here a little translation trick
    //to make sure the widgets rotate around the center
    QTransform t;

    item->menuPosition = widgets.size();
    item->yRot = -20 + 20*(item->menuPosition);
    item->xTrans = 200*(item->menuPosition - 1);

    t.translate(512 , 0);
    t.rotate(item->yRot, Qt::YAxis);
    t.translate(-512 + item->xTrans, 0);
    item->setTransform(t);

    //Add the pixmap to the signal map, which will let us switch menus
    mapper.setMapping(item,uicfile);
    connect(item,SIGNAL(clicked()),
            &mapper,SLOT(map()));

    //There is a few widgets that have automatic updates (ei: loading website)
    //So loop through all the children, find the ones you want and connect the
    // signals via the updateMapper...
    QList<QWidget*> children=tmp->findChildren<QWidget *>();
    for (int i=0;i<children.size();i++)
    {
        QObject *obj=children[i];
        QString className=obj->metaObject()->className();
        QList<const char *> reqs=reasonsToUpdate[className];
        for (int j=0;j<reqs.size();j++)
        {
            updateMapper.setMapping(obj,uicfile);
            connect(obj,reqs[j],
                    &updateMapper,SLOT(map()));
        }
    }

    //Save it all to our fancy dancy maps
    ALayout infostruct;
    infostruct.uicfile=uicfile;
    infostruct.description=description;
    infostruct.item=item;
    infostruct.widget=tmp;

    if (description!="")
    { //If there is no description, then don't bother setting up the stuff..
        infostruct.descItem=descItem;
        infostruct.descTimer=new QTimeLine;
        infostruct.descTimer->setDuration(500);
        infostruct.descTimer->setCurveShape(QTimeLine::EaseInCurve);
        infostruct.descAnimation=new QGraphicsItemAnimation;
        infostruct.descAnimation->setTimeLine(infostruct.descTimer);
        infostruct.descAnimation->setItem(descItem);
        infostruct.descAnimation->setPosAt(0,QPointF(400,100));
        infostruct.descAnimation->setPosAt(1,QPointF(-descItem->boundingRect().width(),100));
        connect(item,SIGNAL(mouseEnter()),
                infostruct.descTimer,SLOT(toggleDirection()));

        connect(item,SIGNAL(mouseEnter()),
                infostruct.descTimer,SLOT(start()));

        connect(item,SIGNAL(mouseLeave()),
                infostruct.descTimer,SLOT(toggleDirection()));

        connect(item,SIGNAL(mouseLeave()),
                infostruct.descTimer,SLOT(start()));

        connect(&timer,SIGNAL(valueChanged(qreal)),
                item,SLOT(itemTransformStep(qreal)));

        connect(this,SIGNAL(currentMenuAnimationForward()),
                item,SLOT(setMenuAnimationDirectionForward()));

        connect(this,SIGNAL(currentMenuAnimationBackward()),
                item,SLOT(setMenuAnimationDirectionBackward()));


    }

    widgets[uicfile]=infostruct;

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

    //Restore background position
    background->setPos(-(background->sceneBoundingRect().width()-1024)/2,0);

    //Reload pixmap
    widgets[current].widget->setEnabled(false);
    QPixmap ss=QPixmap::grabWidget(widgets[current].widget);
    widgets[current].item->setPixmap(ss);

    setUpdatesEnabled(false);
    widgets[current].widget->hide();
    layout.removeWidget(widgets[current].widget);
    menuWidget.show();
    setUpdatesEnabled(true);

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


    emit currentMenuAnimationBackward();
    timer.start();



    previous=current;
    current=QString();
}


void ABase::changeToLevel(const QString& uicfile)
{
    if (timer.state()==QTimeLine::Running) return; //TODO Allow changing levels in mid transition

    if (uicfile==current) return; //Arealdy there...
    else if (!current.isEmpty()) //We are right now at some widget. Switch to the graphics view...
    {
        widgets[current].widget->setEnabled(false);

        //Update the pixmap, in case we changed something..
        QPixmap ss=QPixmap::grabWidget(widgets[current].widget);
        widgets[current].item->setPixmap(ss);

        //Swap...
        setUpdatesEnabled(false);
        layout.removeWidget(widgets[current].widget);
        widgets[current].widget->hide();
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


    animation.setPosAt(1,-widgets[uicfile].item->pos());

    emit currentMenuAnimationForward();
    timer.start();

    //For some reason setting previous to current here would make uicfile a "". Don't ask...
    QString tmp=current;
    current=uicfile;
    previous=tmp;
}

void ABase::animationFinished()
{
    if (current.isEmpty()) return;

    widgets[current].widget->setEnabled(true);

    setUpdatesEnabled(false);
    menuWidget.hide();
    layout.addWidget(widgets[current].widget);
    widgets[current].widget->show();
    setUpdatesEnabled(true);
}

void ABase::updatePixmap(const QString uifile)
{
    if (current==uifile) return; // No need to update the pixmap if the layout is currently being displayed
    QPixmap ss=QPixmap::grabWidget(widgets[uifile].widget);
    widgets[uifile].item->setPixmap(ss);
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
    if (widgets.size()==0) return QPoint();

    //Figures out where the widget group would go if it were scaled by 0.2
    // and centered
    QPointF ret= QPointF((1024-widgetGroup.sceneBoundingRect().width())/2,
                         (768-widgetGroup.sceneBoundingRect().height())/2 - 200);

    return ret;
}

QPointF ABase::calculateBackgroundPosition()
{
    return QPointF(-(background->sceneBoundingRect().width()-1024)/2,0);
}

bool ABase::eventFilter(QObject *obj, QEvent *event)
{
    if (!current.isEmpty()) return false; //Don't care if this no mouse

    if (obj==&menu)
    {
        if (event->type()==QEvent::GraphicsSceneMouseMove)
        {
            QGraphicsSceneMouseEvent *mEvent=(QGraphicsSceneMouseEvent*)event;

            //Need to figure out the bounding rectangle of our group widget
            QPointF lastPos=mEvent->lastScenePos();
            QRectF rect=widgetGroup.sceneBoundingRect();
            QSizeF size=widgetGroup.childrenBoundingRect().size()*0.25;
            rect.setSize(size); // For some reason I keep getting size of 0x0 by default
            rect.moveLeft(rect.x()-size.width()/2); //The top left of the bounding rect is the center of the actual widget group...

            if (rect.contains(lastPos)) //If we are hovering over top of the pixmaps, scroll
            {
                //Make sure no animation is running
                parallaxTimer.stop();
                parallaxTimerBg.stop();

                //Figure out how much we moved the mouse by
                QPointF delta=mEvent->lastScenePos()-mEvent->scenePos();

                //Move everything by the above change, but opposite and scaled
                delta.setY(0);
                widgetGroup.setPos(widgetGroup.pos()+0.1*delta);
                background->setPos(background->pos()+0.04*delta);
                menuWidget.horizontalScrollBar();
            }
            else if (parallaxTimer.state()!=QTimeLine::Running && background->pos()!=calculateBackgroundPosition()) //Restore to initial position using an animator, but only if it is not already there and an animation is not running already..
            {
                parallaxAnimation.setPosAt(0,widgetGroup.pos());
                parallaxAnimation.setPosAt(1,calculateScaledWidgetGroupPosition());
                parallaxTimer.setCurrentTime(0);
                parallaxTimer.start();

                parallaxAnimationBg.setPosAt(0,background->pos());
                parallaxAnimationBg.setPosAt(1,calculateBackgroundPosition());
                parallaxTimerBg.setCurrentTime(0);
                parallaxTimerBg.start();
            }
        }
    }
    return false;
}

void ABase::on_MenuButton_clicked()
{
    changeToMenu();
}

void ABase::on_AnalyzeButton_clicked()
{
    changeToLevel("geometry.ui");
}

void ABase::on_MenuButton2_clicked()
{
    changeToMenu();
}

void ABase::on_AnalyzeButton2_clicked()
{
    changeToLevel("geometry.ui");
}

void ABase::on_MenuButton_activated()
{
    changeToMenu();
}

void ABase::on_actionGetting_Started_activated()
{
    changeToLevel("wikibrowser.ui");
}

void ABase::on_QuitButton_activated()
{
    close();
}

