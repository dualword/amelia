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

ABase::ABase( QWidget *parent )
  : QMainWindow(parent),geoNotLoaded(true)
{
  //Set to the initial size.
  resize(1024,730);
  
  //Create a widget to act as the central widget.
  center=new QWidget(this);
  setCentralWidget(center);
 
  //Create a widget that acts as our moving area. All virtual screens are added here!
  area=new QWidget(center);
  area->resize(1024,730);
  
  //This grid layout will contain any layouts. The size of each virtual screen will approximatelly be 1024x730 for now
  layout=new QGridLayout(area);
  layout->setContentsMargins(0,0,0,0);
}

ABase::~ABase() { }

void ABase::addLevel(QString uicfile)
{
  //Increase the size of the area widget
  area->resize(1024*(screens.size()+1),730);

  //Set the initial layout to the first layout loaded
  if(current.isEmpty()) current=uicfile;

  //Everything else should be an UI file (for now)
  AUILoader loader;
  QFile file(":/ui/"+uicfile);
  file.open(QFile::ReadOnly);
  QWidget *tmp = loader.load(&file);
  file.close();

  VirtualScreen *vsinfo=new VirtualScreen;
  if (!tmp)
    {
      QMessageBox::critical(this,"Failed to change UI","There was a problem loading the UI named \""+uicfile+"\". This could be caused by a corrupt installation.");
    }
  else if (tmp->inherits("QMainWindow"))  //This is a Main Window, copy
    {
      //central widgets and everything!
      vsinfo->widget=((QMainWindow*)tmp)->centralWidget();
      
      vsinfo->menubar=((QMainWindow*)tmp)->menuBar();
      vsinfo->menubar->installEventFilter(this); //Event filter used to block deletion by setMenuBar()

      //TODO Figure out how to remove a status bar from the designer file
      // Currently, even if we don't have a status bar one is created...
      // So for now, status bars have been disabled from all of Amelia!!
      //setStatusBar(((QMainWindow*)tmp)->statusBar());
      
      //Setup the toolbars of the main window
      vsinfo->toolbars=((QMainWindow*)tmp)->findChildren<QToolBar *>();

      //There is a problem with the QActions being lost in the ether
      // (or maybe there is a black hole in one of the included events
      // that swallows them...) if the parent of QToolBar is removed.
      // That's why we lock
      // their parents to the central widget...
      QList<QAction*> acts=tmp->findChildren<QAction *>();
      for(int i=0;i<acts.size();i++)
	{
	  acts[i]->setParent(vsinfo->widget);
	}
      
      
      //Note: We do not try to delete the old QMainWindow, because
      //doing so will remove menu actions too.
    }
  else
    {
      //Classic QWidget. Just set it to center.
      vsinfo->widget=tmp;
      vsinfo->menubar=0;
    }
  
  //Setup initial dimsensions and disable everything by default
  vsinfo->widget->setParent(area);
  vsinfo->widget->setMinimumSize(1024,500);
  vsinfo->widget->setMaximumSize(1024,730);
  vsinfo->widget->move(1024*screens.size(),0);
  vsinfo->widget->setEnabled(false);

  //TODO Implement maximum number of columns!
  //int col=screens.size()%3;
  //int row=screens.size()/3;
  layout->addWidget(vsinfo->widget,0,screens.size());
  layout->setAlignment(vsinfo->widget,Qt::AlignTop);
  
  screens[uicfile]=vsinfo;

  //Append stylesheet...
  setStyleSheet(styleSheet()+tmp->styleSheet());
}

void ABase::changeToLevel(QString uicfile)
{
  //Remove old bars, status bars and blah..
  if(menuBar()) menuBar()->setParent(new QWidget()); //manubars need a parent even if detached to work
  setMenuBar(0);
  setStatusBar(0);
  QList<QToolBar *> toolbars=findChildren<QToolBar *>();
  for (int i=0;i<toolbars.size();i++)
    {
      removeToolBar(toolbars[i]);
    }

  //Disable the current widget
  screens[current]->widget->setEnabled(false);

  current=uicfile;
  //Figure out the needed delta amount. Note: the position of the widget!=the final position of the area since the top of the area is not always the top of the widget.
  delta=screens[uicfile]->widget->pos()+area->pos();
  
  startTimer(10);
}

void ABase::timerEvent(QTimerEvent *event)
{

  QPoint at=area->pos();
  int length=delta.manhattanLength();

  //Are we there yet?
  if(length==0)
    {
      VirtualScreen *vs=screens[current];
      
      int height=0;
      //Setup menus and toolbars
      if(vs->menubar)
        {
          vs->menubar->setParent(this);
          setMenuBar(vs->menubar);
        }
      for (int i=0;i<vs->toolbars.size();i++)
        {
          QToolBar* bar=vs->toolbars[i];
          addToolBar(bar);
          bar->show(); // Qt hides a toolbar when we remove it. So make sure it's shown.
        }
      
      //Fix the size of the widget to accomodate any added menus or toolbars
      //I'm not sure where the -25 comes from, but it is there if we add
      //menubars or toolbars
      QSize size=center->size();
      size.setHeight(size.height()-25);
      vs->widget->setMinimumSize(size);
      vs->widget->setMaximumSize(size);
      
      //Enable it...
      vs->widget->setEnabled(true);

      //HACK (ugly) setup the conenctions of the ALayerGUI
      //TODO figure out a more general way to do this
      if(current=="geometry.ui" && geoNotLoaded)
	{
	  ((ALayerGUI*)vs->widget)->setupElements();
	  geoNotLoaded=false;
	}
      
      killTimer(event->timerId());
      return;
    }

  //Some math 
  int steps=(abs(delta.x())>abs(delta.y()))?delta.x():delta.y();
  steps=abs(steps)/100; // This sets the speed of the animation. Bigger number on the bottom means faster
  QPoint smallDelta(delta.x()/steps,delta.y()/steps);
  at-=smallDelta;
  delta-=smallDelta; //Update needed delta

  area->move(at); //MOVE MOVE!
}

bool ABase::eventFilter(QObject *obj, QEvent *event)
{
  if(obj->inherits("QMenuBar"))
    {
      //We don't want to delete the menubars, instead we want to cache them. However they are deleted automatically by setting a new menubar,
      // so we need to block this.
      //TODO There might be some time when we DO want to delete a menubar
      if(event->type()==QEvent::DeferredDelete) return true;
    }
  return QMainWindow::eventFilter(obj,event);
}

//TEST TEST Lets me quickly test the animations without having to add buttons TEST TEST
void ABase::keyPressEvent(QKeyEvent *event)
{
  if(event->key()==Qt::Key_Escape)
    {
      QList<QString> keys=screens.keys();
      int idx=keys.indexOf(current);
      idx=(idx+1)%keys.size();
      changeToLevel(keys[idx]);
    }
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
