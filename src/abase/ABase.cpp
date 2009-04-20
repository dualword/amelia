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

#include <AMELIA.h>

#include <QUiLoader>
#include <QGraphicsSceneMouseEvent>
#include <QGLWidget>

ABase::ABase( QWidget *parent )
  : QMainWindow(parent),_fakeCentralWidget(0)
{
    //Set to the initial size.
    resize(1024,768);
  
    layout.setContentsMargins(0,0,0,0);
    center.setLayout(&layout);

    //Some settings for the scene
    menu.setSceneRect(0,0,1024,768); //Aka disable scrolling
    menu.installEventFilter(this);

    //Initialize the background
    background=menu.addPixmap(QPixmap(":/media/CommandersBridge.png"));
    background->setZValue(0);
    background->setPos(calculateBackgroundPosition());

    //Add the buttons
    buttonFont.setBold( true );
    buttonFont.setPointSize( 24 );
    buttonFont.setStyleHint( QFont::Times );
    buttonFont.setStyleStrategy( QFont::PreferAntialias );
    buttonGroup.setHandlesChildEvents(false);
    buttonGroup.setZValue(2);
    buttonGroup.setHorizontalAlignment(Qt::AlignHCenter);
    buttonGroup.setPos(1024/2,700);
    menu.addItem(&buttonGroup);
    //Also add a quit button :)
    quit.setText("Quit");
    quit.setFont(buttonFont);
    quit.setAcceptHoverEvents(true);
    buttonGroup.addToGroup(&quit);
    connect(&quit,SIGNAL(clicked()),
	    this,SLOT(close()));
    //This mapper takes care of the button signals
    buttonMapper.setParent(this);
    connect(&buttonMapper,SIGNAL(mapped(const QString &)),
	    this,SLOT(changeToGroup(const QString &)));
    

    //Timers for restoring parallax animations
    parallaxTimer.setDuration(1000);
    parallaxTimer.setCurveShape(QTimeLine::EaseInOutCurve);
    parallaxAnimation.setTimeLine(&parallaxTimer);
    parallaxTimerBg.setDuration(1000);
    parallaxTimerBg.setCurveShape(QTimeLine::EaseInOutCurve);
    parallaxAnimationBg.setItem(background);
    parallaxAnimationBg.setTimeLine(&parallaxTimerBg);

    //Make the widget without scrollbars and display it
    menuWidget.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    menuWidget.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setupViewport();

    menuWidget.setStyleSheet("border: none"); //Makes the border around the graphics view dissapear
    setFakeCentralWidget(&menuWidget);

    setCentralWidget(&center);

    //This mapper will be used for connecting pixmaps to changeToMonitor
    connect(&mapper,SIGNAL(mapped(const QString&)),
            this,SLOT(changeToMonitor(const QString&)));

    //Used by transition animations
    animation.setTimeLine(&timer);
    animation.setScaleAt(0,0.25,0.25);
    animation.setPosAt(0,QPointF(1024/2,150));
    
    animation.setScaleAt(1,1,1);
    animation.setPosAt(1,QPointF(1024/2,0));
}

ABase::~ABase() { }

void ABase::load()
{
  AMELIA* amelia=pluginBase();
  connect(amelia,SIGNAL(allPluginsLoaded()),
	  this,SLOT(showEverything()));

  show();
}

void ABase::setupViewport()
{
    QWidget *viewport;

#ifdef Q_WS_WIN
    viewport=new QWidget(&menuWidget);
    viewport->setAttribute(Qt::WA_MSWindowsUseDirect3D);
    menuWidget.setCacheMode(QGraphicsView::CacheNone);
#else
    //viewport = new QGLWidget(/*QGLFormat(QGL::SampleBuffers)*/);
    viewport = new QWidget(&menuWidget);
    menuWidget.setOptimizationFlag(QGraphicsView::DontClipPainter);
#endif
    menuWidget.setViewport(viewport);
    menuWidget.setRenderHint( QPainter::Antialiasing );
}

void ABase::setFakeCentralWidget(QWidget *wdg)
{
  setUpdatesEnabled(false);
  if(_fakeCentralWidget)
    {
      layout.removeWidget(_fakeCentralWidget);
      _fakeCentralWidget->hide();

#ifdef Q_WS_MAC
      //The menu bar is only set on MacOSX, because MacOSX has the universal top menu bar.
      // On Linux, playing with the menubar causes offset problems with the graphics view

      //Store the current menubar in the current fake widget
      if(_fakeCentralWidget->inherits("QMainWindow"))
	{
	  QMainWindow *mainWin=(QMainWindow*)_fakeCentralWidget;
	  mainWin->setMenuBar(menuBar());
	  mainWin->menuBar()->hide();
	  setMenuBar(0);
	}
#endif //Q_WS_MAC
    }

#ifdef Q_WS_MAC
  //Set the new fake widget's menubar to the main window's menubar
  if(wdg->inherits("QMainWindow"))
    {
      QMainWindow *mainWin=(QMainWindow*)wdg;
      setMenuBar(mainWin->menuBar());
      menuBar->show();
    }
#endif //Q_WS_MAC

  layout.addWidget(wdg);
  wdg->show();
  _fakeCentralWidget=wdg;
  setUpdatesEnabled(true);
}

QWidget* ABase::fakeCentralWidget()
{
  return _fakeCentralWidget;
}

void ABase::addMonitor(QString name,QString group,QWidget *tmp,QString description,Qt::Alignment align)
{    
  //Make sure to disable the widget
  tmp->resize(1024,768);
  
  if(!monitorGroups.contains(group))
    { //Create a new group
      addGroup(group);
    }

  AMonitor* item=new AMonitor(name,description,tmp,monitorGroups[group]);

  monitorGroups[group]->addMonitor(name,item,align);

  //Add the pixmap to the signal map, which will let us switch menus
  mapper.setMapping(item,group+"/"+name);
  connect(item,SIGNAL(clicked()),
	  &mapper,SLOT(map()));
}

void ABase::addGroup(QString groupName)
{
  if(monitorGroups.contains(groupName)) return; //Ignore if already exists
  
  monitorGroups[groupName]=new AMonitorGroup(); //Create
  
  QGraphicsClickableSimpleTextItem *button=new QGraphicsClickableSimpleTextItem(groupName);
  button->setFont(buttonFont);
  buttonGroup.addToGroup(button,Qt::AlignLeft);

  menu.addItem(monitorGroups[groupName]); //Add to scene

  //Connect it to the mapper, which connects it to the switching code
  connect(button,SIGNAL(clicked()),
	  &buttonMapper,SLOT(map()));
  buttonMapper.setMapping(button,groupName);
}

void ABase::changeToMenu()
{
  if (timer.state()==QTimeLine::Running) return; //TODO Allow changing levels in mid transition
  
  if (!currentMonitor.isEmpty()) //Not menu yet, update stuff
    {
      //Restore background position
      background->setPos(-(background->sceneBoundingRect().width()-1024)/2,0);
      
      AMonitor* monitor=monitorGroups[currentGroup]->monitor(currentMonitor);

      setFakeCentralWidget(&menuWidget);
      monitor->restoreWidget();
    }
  
  monitorGroups[currentGroup]->setHandlesChildEvents(true);
  timer.setDirection(QTimeLine::Backward);
  timer.setTimeLine(monitorGroups[currentGroup]->timeLine());
  monitorGroups[currentGroup]->setSelected(QString());
    
  
  previousMonitor=currentMonitor;
  currentMonitor=QString();
}


void ABase::changeToMonitor(const QString& path)
{
  if(path.isEmpty())
    { //Change to menu if the path is empty. Simplifies a lot of signal mappers.
      changeToMenu(); 
      return;
    }

  QStringList parts=path.split("/");
  if(parts.size()!=2)
    {
      qDebug() << "Invalid monitor change invocation: " << path;
      return;
    }
  
  QString monitor=parts[1];
  QString group=parts[0];
  
  qDebug() << "Change to monitor: " << group << "/" << monitor;

  if(currentGroup!=group) return; //TODO Allow changing of groups
  if (monitor==currentMonitor) return; //Arealdy there...
  if (timer.state()==QTimeLine::Running) return; //TODO Allow changing levels in mid transition

  monitorGroups[group]->setHandlesChildEvents(true);

  if(!currentMonitor.isEmpty()) //We are right now at some widget. Switch to the graphics view...
    {
      AMonitor* current=monitorGroups[currentGroup]->monitor(currentMonitor);
      
      //Swap...
      setFakeCentralWidget(&menuWidget);
      current->restoreWidget();
    }
  else
    {
      timer.setDirection(QTimeLine::Forward);
      timer.setTimeLine(monitorGroups[group]->timeLine());
    }

  monitorGroups[group]->setSelected(monitor);

  //For some reason setting previous to current here would make uicfile a "". Don't ask...
  QString tmp=currentMonitor;
  currentMonitor=monitor;
  previousMonitor=tmp;
}

void ABase::changeToGroup(const QString& group)
{
  //If the current group is what we are changing to, then just hide it.
  // This is currently the default response to help testing
  
  if(!currentGroup.isEmpty())
    {
      disconnect(monitorGroups[currentGroup],SIGNAL(layoutReady()),
		 this,SLOT(animationFinished()));
    }

  if(currentGroup==group)
    {
      monitorGroups[group]->hide();
      currentGroup="";
      return;
    }
  
  if(!currentGroup.isEmpty()) monitorGroups[currentGroup]->hide(); //Hide currently shown group
  monitorGroups[group]->show(); //Show it
  currentGroup=group; //Update the name of the currently shown group
  
  animation.setItem(monitorGroups[group]);     
  connect(monitorGroups[group],SIGNAL(layoutReady()),
	  this,SLOT(animationFinished()));
}

void ABase::showEverything()
{
  menuWidget.setScene(&menu);
}

void ABase::animationFinished()
{
  monitorGroups[currentGroup]->setHandlesChildEvents(false);
  timer.setTimeLine(0);

  if (currentMonitor.isEmpty()) return;
  AMonitor* monitor=monitorGroups[currentGroup]->monitor(currentMonitor);
  QWidget *widget=monitor->widget();
  monitor->storeWidget();

  setFakeCentralWidget(widget);
}

//TEST TEST Lets me quickly test the animations without having to add buttons TEST TEST
void ABase::keyPressEvent(QKeyEvent *event)
{
  if (event->key()==Qt::Key_Escape)
    {
      if (currentMonitor.isEmpty() && !previousMonitor.isEmpty())
	changeToMonitor(currentGroup+"/"+previousMonitor);
      else
	changeToMenu();
    }
  
  if (event->key()==Qt::Key_Delete)
    {
      QList<QString> keys=monitorGroups[currentGroup]->monitors().keys();
      int idx=keys.indexOf(currentMonitor);
      idx=(idx+1)%keys.size();
      changeToMonitor(currentGroup+"/"+keys[idx]);
    }

  //What are we gonna do with menu's on Mac OS X?
  if (event->key()==Qt::Key_F)
  {
    setWindowState(windowState() ^ Qt::WindowFullScreen);
  }
}

QPointF ABase::calculateBackgroundPosition()
{
  return QPointF(-(background->sceneBoundingRect().width()-1024)/2,0);
}

void ABase::resizeEvent(QResizeEvent* event)
{
  qreal sx=((float)event->size().width())/1024;
  qreal sy=((float)event->size().height())/768;
  menuWidget.resetTransform();
  menuWidget.scale(sx,sy);
}

bool ABase::eventFilter(QObject *obj, QEvent *event)
{
  if (!currentMonitor.isEmpty()) return false; //Don't care if this no mouse
  if (currentGroup.isEmpty()) return false; //Don't care if this no mouse
  if (timer.state() == QTimeLine::Running) return false; //Don't care if switching menus..
  
  if (obj==&menu)
    {
      if (event->type()==QEvent::GraphicsSceneMouseMove)
        {
            QGraphicsSceneMouseEvent *mEvent=(QGraphicsSceneMouseEvent*)event;
	    
	    //What are we gonna parallax?
	    AMonitorGroup* widgetGroup=monitorGroups[currentGroup];
	    
            //Need to figure out the bounding rectangle of our group widget
            QPointF lastPos=mEvent->lastScenePos();
            QRectF childrenRect=widgetGroup->childrenBoundingRect();
            QRectF rect=widgetGroup->sceneTransform().mapRect(childrenRect);
	    
            if (rect.contains(lastPos)) //If we are hovering over top of the pixmaps, scroll
	      {
                //Make sure no animation is running
                parallaxTimer.stop();
                parallaxTimerBg.stop();
		
                //Figure out how much we moved the mouse by
                QPointF delta=mEvent->lastScenePos()-mEvent->scenePos();
		
                //Move everything by the above change, but opposite and scaled
                delta.setY(0);
                widgetGroup->setPos(widgetGroup->pos()+0.1*delta);
                background->setPos(background->pos()+0.04*delta);
                menuWidget.horizontalScrollBar();
            }
            else if (parallaxTimer.state()!=QTimeLine::Running && background->pos()!=calculateBackgroundPosition()) //Restore to initial position using an animator, but only if it is not already there and an animation is not running already..
            {
                parallaxAnimation.setPosAt(0,widgetGroup->pos());
		parallaxAnimation.setPosAt(1,widgetGroup->calculateScaledWidgetGroupPosition());
		parallaxAnimation.setItem(widgetGroup);
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

Q_EXPORT_PLUGIN2(ABase, ABase)
