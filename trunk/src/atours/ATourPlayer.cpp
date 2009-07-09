#include "ATourPlayer.h"

#include <QDebug>
#include <QApplication>
#include <QHBoxLayout>
#include <QPushButton>

ATourPlayer::ATourPlayer()
  :currentTour(0),currentBlock(0),controls(0),playerControls(0),timeLine(1)
{
  connect(&timeLine,SIGNAL(finished()),
	  this,SLOT(tourFinished()));

  QApplication::instance()->installEventFilter(this);
}

void ATourPlayer::setupElements()
{
  geoplugin=(AGeoPlugin *)AMELIA::global->plugin("AGeometry");  
  
  baseplugin=(ABase*)AMELIA::global->plugin("ABase");  
  geo=(AGeometry*)geoplugin->findWidget("Geometry");
  
  controls=new ASlidyManager(baseplugin,50,Qt::AlignTop);
  controls->setWidth(baseplugin->width()-200);
  controls->setPosition(100);

  playerControls=new QWidget();
  playerControls->setObjectName("TourControls");
  QLayout *playerLayout=new QHBoxLayout(playerControls);
  playerLayout->setContentsMargins(0,0,0,0);

  QPushButton *backwardsButton=new QPushButton();
  backwardsButton->setText("Backward");
  connect(backwardsButton,SIGNAL(clicked()),
	  this,SLOT(backward()));
  playerLayout->addWidget(backwardsButton);
  
  QPushButton *playPauseButton=new QPushButton();
  playPauseButton->setText("Play/Pause");
  connect(playPauseButton,SIGNAL(clicked()),
	  this,SLOT(toggle()));
  playerLayout->addWidget(playPauseButton);

  QPushButton *stopButton=new QPushButton();
  stopButton->setText("Stop");
  connect(stopButton,SIGNAL(clicked()),
	  this,SLOT(stop()));
  playerLayout->addWidget(stopButton);

  QPushButton *forwardsButton=new QPushButton();
  forwardsButton->setText("Forward");
  connect(forwardsButton,SIGNAL(clicked()),
	  this,SLOT(forward()));
  playerLayout->addWidget(forwardsButton);

  timeControl=new QSlider(Qt::Horizontal);
  connect(&timeLine,SIGNAL(frameChanged(int)),
	  timeControl,SLOT(setValue(int)));
  connect(timeControl,SIGNAL(sliderMoved(int)),
	  this,SLOT(seek(int)));
  connect(timeControl,SIGNAL(sliderPressed()),
	  this,SLOT(pause()));
  connect(timeControl,SIGNAL(sliderReleased()),
	  this,SLOT(resume()));
  playerLayout->addWidget(timeControl);

  timeShow=new QSpinBox();
  connect(&timeLine,SIGNAL(frameChanged(int)),
	  timeShow,SLOT(setValue(int)));
  playerLayout->addWidget(timeShow);



  QWidget *blocksControls=new QWidget();
  blocksControls->setObjectName("TourBlockControls");
  QHBoxLayout *blocksOuterLayout=new QHBoxLayout(blocksControls);
  blocksOuterLayout->setContentsMargins(0,0,0,0);
  connect(&blocksMapper,SIGNAL(mapped(int)),
	  this,SLOT(playBlock(int)));
  blocksLayout=new QHBoxLayout();
  QPushButton *closeButton=new QPushButton();
  closeButton->setText("Close");
  connect(closeButton,SIGNAL(clicked()),
	  controls,SLOT(hideWidget()));

  blocksOuterLayout->addLayout(blocksLayout);
  blocksOuterLayout->addWidget(closeButton);
  
  controls->addWidget(playerControls,"",false);
  controls->addWidget(blocksControls,"",false);
}

QTimeLine::State ATourPlayer::state()
{
  return timeLine.state();
}

void ATourPlayer::play(ATour *tour)
{
  if(timeLine.state()!=QTimeLine::NotRunning)
    {
      timeLine.stop();
    }

  for(int i=blocksLayout->count()-1;i>=0;i--)
    {
      QWidget *wdg=blocksLayout->takeAt(i)->widget();
      if(wdg)
	{
	  blocksLayout->removeWidget(wdg);
	  blocksMapper.removeMappings(wdg);
	  wdg->deleteLater();
	}
    }

  for(int i=0;i<tour->blockCount();i++)
    {
      QString blockName="Block "+QString::number(i+1);
      QPushButton *b1=new QPushButton;
      b1->setText(blockName);
      blocksLayout->addWidget(b1);
      blocksMapper.setMapping(b1,i);
      connect(b1,SIGNAL(clicked()),
	      &blocksMapper,SLOT(map()));
    }
  
  controls->showWidget(1);

  geoplugin->unloadEvent();
  
  currentTour=tour;
}

void ATourPlayer::playBlock(int idx)
{
  if(currentTour==0) return;
  
  controls->showWidget(0);

  if(currentBlock)
    disconnect(&timeLine,SIGNAL(frameChanged(int)),
	       currentBlock,SLOT(updateFrame(int)));
  
    
  currentBlock=currentTour->block(idx);
  currentBlock->prepare();

  connect(&timeLine,SIGNAL(frameChanged(int)),
	  currentBlock,SLOT(updateFrame(int)));

  timeControl->setMaximum(currentBlock->duration());
  timeShow->setMaximum(currentBlock->duration());

  timeLine.setDuration(currentBlock->duration());
  timeLine.setFrameRange(0,currentBlock->duration());
  timeLine.setCurveShape(QTimeLine::LinearCurve);
  timeLine.setDirection(QTimeLine::Forward);
  timeLine.setCurrentTime(0);
  timeLine.start();

  emit tour_started();
}

void ATourPlayer::pause()
{
  timeLine.setPaused(true);
}

void ATourPlayer::resume()
{
  timeLine.resume();
}

void ATourPlayer::toggle()
{
  if(timeLine.state()==QTimeLine::Running)
    timeLine.setPaused(true);
  else if(timeLine.state()==QTimeLine::Paused)
    timeLine.resume();
}

void ATourPlayer::stop()
{
  timeLine.stop();
  tourFinished();
}

void ATourPlayer::seek(int to)
{
  if(sender()==&timeLine) return;

  timeLine.setCurrentTime(((double)to)/((double)timeLine.endFrame())*timeLine.duration());
}

void ATourPlayer::backward()
{
  timeLine.setDirection(QTimeLine::Backward);
}

void ATourPlayer::forward()
{
  timeLine.setDirection(QTimeLine::Forward);
}

void ATourPlayer::tourFinished()
{
  currentBlock->cleanup();

  emit tour_stopped();

  controls->showWidget(1);
}

bool ATourPlayer::eventFilter(QObject *obj, QEvent *event)
{
  // If the click is on one of the controls, don't filter
  if(obj==playerControls || hasParent(obj,playerControls)) return false;


  // Resize the player controls if the base plugin is resized
  if(obj==baseplugin)
    {
      if(controls && event->type()==QEvent::Resize)
	controls->setWidth(baseplugin->width()-200);
    }
  
  if(timeLine.state()!=QTimeLine::Running)
    {  // If not running, hide the tour controls on a click outside of the controls
      if(obj!=playerControls && !hasParent(obj,playerControls) && controls && controls->visibleId()!=-1)
	{
	  switch(event->type())
	    {
	    case QEvent::GraphicsSceneMouseDoubleClick:
	    case QEvent::GraphicsSceneMousePress:
	    case QEvent::GraphicsSceneMouseRelease:
	    case QEvent::KeyPress:
	    case QEvent::KeyRelease:
	    case QEvent::MouseButtonDblClick:
	    case QEvent::MouseButtonPress:
	    case QEvent::MouseButtonRelease:
	      controls->hideWidget();
	      break;
	    default:
	      break;
	    }
	}
      return QObject::eventFilter(obj,event);
    }
  else
    { //We are running, so block all filters!
      switch(event->type())
	{
	case QEvent::FocusIn:
	case QEvent::FocusOut:
	case QEvent::GraphicsSceneHoverEnter:
	case QEvent::GraphicsSceneHoverLeave:
	case QEvent::GraphicsSceneHoverMove:	
	case QEvent::GraphicsSceneMouseDoubleClick:
	case QEvent::GraphicsSceneMouseMove:
	case QEvent::GraphicsSceneMousePress:
	case QEvent::GraphicsSceneMouseRelease:
	case QEvent::HoverEnter:
	case QEvent::HoverLeave:
	case QEvent::HoverMove:
	case QEvent::KeyPress:
	case QEvent::KeyRelease:
	case QEvent::MouseButtonDblClick:
	case QEvent::MouseButtonPress:
	case QEvent::MouseButtonRelease:
	case QEvent::MouseMove:
	case QEvent::MouseTrackingChange:
	  return true;
	default:
	  return QObject::eventFilter(obj,event);
	}
    }
}

 bool ATourPlayer::hasParent(QObject *obj,QObject *parent)
 {
   if(obj==0) return false;
   if(obj->parent()==parent) return true;
   return hasParent(obj->parent(),parent);
 }

