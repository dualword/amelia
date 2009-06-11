#include "ATourPlayer.h"

#include <QDebug>
#include <QApplication>
#include <QHBoxLayout>
#include <QPushButton>

ATourPlayer::ATourPlayer()
  :currentTour(0),currentBlock(0),timeLine(1)
{
  connect(&timeLine,SIGNAL(finished()),
	  this,SLOT(tourFinished()));

  QApplication::instance()->installEventFilter(this);
}

void ATourPlayer::setupElements()
{
  geoplugin=(AGeoPlugin *)AMELIA::global->plugin("AGeometry");  
  ABase *baseplugin=(ABase*)AMELIA::global->plugin("ABase");  
  geo=(AGeometry*)geoplugin->findWidget("Geometry");
  
  //controls=new ASlidyManager(geoplugin->findWidget("LayerGUI"),Qt::AlignTop);
  controls=new ASlidyManager(baseplugin,Qt::AlignTop);
  controls->setMaxSize(50);
  qDebug() << "USE " << baseplugin;

  playerControls=new QWidget();
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

  QWidget *blocksControls=new QWidget();
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
  
  controls->addWidget(playerControls,"Controls");
  controls->addWidget(blocksControls,"Blocks");
  
  /*QObject::connect (&tourManager, SIGNAL(tour_camera(AGeometry::CameraMode)),
		    geo, SLOT(setViewport(AGeometry::CameraMode)));
  QObject::connect (&tourManager, SIGNAL(tour_loadfile(QString)),
		    geoplugin, SLOT(loadEvent(QString)));
  QObject::connect (&tourManager, SIGNAL(tour_stopped()),
		    this, SLOT(endTour()));
		    QObject::connect (&tourManager, SIGNAL(tour_button(char *)),
  this, SLOT(pressButton(char *)));*/
  //QObject::connect (tourManager, SIGNAL(tour_ptchange(int)), geo->XmlEvt, SLOT(PtCutoff(int)));

  //QCoreApplication::installEventFilter (this);

}

QTimeLine::State ATourPlayer::state()
{
  return timeLine.state();
}

void ATourPlayer::play(ATour *tour)
{
  if(timeLine.state()!=QTimeLine::QTimeLine::NotRunning)
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
  connect(&timeLine,SIGNAL(frameChanged(int)),
	  currentBlock,SLOT(updateFrame(int)));
  currentBlock->reset();  

  timeLine.setDuration(currentBlock->duration());
  timeLine.setFrameRange(0,currentBlock->duration());
  timeLine.setCurveShape(QTimeLine::LinearCurve);
  timeLine.setDirection(QTimeLine::Forward);
  timeLine.setCurrentTime(0);
  timeLine.start();

  geo->setViewport(AGeometry::Cam3D);

  APoint3D oldPos=geo->cameraPosition();
  APoint3D oldTar=geo->cameraTarget();  
  geo->setCamera(AGeometry::Lock,false);
  geo->setCameraPosition(oldPos);
  geo->setCameraTarget(oldTar);
    
  emit tour_started();
}


void ATourPlayer::toggle()
{
  if(timeLine.state()==QTimeLine::Running)
    timeLine.setPaused(true);
  else if(timeLine.state()==QTimeLine::Paused)
    timeLine.setPaused(false);
}

void ATourPlayer::stop()
{
  timeLine.stop();
  tourFinished();
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
  APoint3D oldPos=geo->cameraPosition();
  APoint3D oldTar=geo->cameraTarget();  
  geo->setCamera(AGeometry::FPS,false);
  geo->setCameraPosition(oldPos);
  geo->setCameraTarget(oldTar);

  emit tour_stopped();

  controls->showWidget(1);
}

bool ATourPlayer::eventFilter(QObject *obj, QEvent *event)
{
  if(timeLine.state()!=QTimeLine::Running) return QObject::eventFilter(obj,event);
  if(obj==playerControls || hasParent(obj,playerControls)) return false;

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

 bool ATourPlayer::hasParent(QObject *obj,QObject *parent)
 {
   if(obj==0) return false;
   if(obj->parent()==parent) return true;
   return hasParent(obj->parent(),parent);
 }

