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
  : QMainWindow(parent)
{
  //Set to the initial size.
  resize(1024,768);
  
  //Create a widget to act as the central widget.
  center=new QWidget(this);
  setCentralWidget(center);
 
  //Create a widget that acts as our moving area. All virtual screens are added here!
  area=new QWidget(center);
  area->resize(1024,768);
  
  //This grid layout will contain any layouts. The size of each virtual screen will approximatelly be 1024x768 for now
  layout=new QGridLayout(area);
  layout->setContentsMargins(0,0,0,0);
}

ABase::~ABase() { }

void ABase::addLevel(QString uicfile)
{
  //Increase the size of the area widget
  area->resize(1024*(screens.size()+1),768);

  //Set the initial layout to the first layout loaded
  if(current.isEmpty()) current=uicfile;

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
  
  //Setup initial dimsensions and disable everything by default
  tmp->setEnabled(false);

  //TODO Implement maximum number of columns!
  //int col=screens.size()%3;
  //int row=screens.size()/3;
  layout->addWidget(tmp,0,screens.size());
  layout->setColumnMinimumWidth(screens.size(),1024);

  //HACK (ugly) setup the conenctions of the ALayerGUI
  //TODO figure out a more general way to do this
  if(uicfile=="geometry.ui")
    ((ALayerGUI*)((QMainWindow*)tmp)->centralWidget())->setupElements();
  
  screens[uicfile]=tmp;
}

void ABase::changeToLevel(QString uicfile)
{
  //Disable the current widget
  screens[current]->setEnabled(false);

  current=uicfile;
  //Figure out the needed delta amount. Note: the position of the widget!=the final position of the area since the top of the area is not always the top of the widget.
  delta=screens[uicfile]->pos()+area->pos();

  startTimer(10);
}

void ABase::timerEvent(QTimerEvent *event)
{

  QPoint at=area->pos();
  int length=delta.manhattanLength();

  //Are we there yet?
  if(length==0)
    {
      //Enable it...
      screens[current]->setEnabled(true);

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
