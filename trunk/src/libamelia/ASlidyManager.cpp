#include "ASlidyManager.h"
#include <QDebug>
#include <QResizeEvent>
#include <QStyleOption>
#include <QPainter>
#include <math.h>

ASlidyManager::ASlidyManager(QWidget *parent,int maxSize,Qt::Alignment align)
  :QObject(parent),mainTimeline(1000),
   showingTimeline(&mainTimeline,&mainTimeline),
   hidingTimeline(&mainTimeline,&mainTimeline),
   _visibleId(-1),_lastVisibleId(-1),
   _position(0),_width(0),_height(0),
   align(align)
{ 
  parent->installEventFilter(this);
  
  showingAnimator.setTimeLine(&showingTimeline);
  hidingAnimator.setTimeLine(&hidingTimeline);
  hidingTimeline.setDirection(QTimeLine::Backward);

  timer.setSingleShot(true);
  timer.setInterval(10000);
  connect(&timer,SIGNAL(timeout()),
	  this,SLOT(timerEvent()));

  // Setup the maximum size of a widget in the direction of the slide.
  // This quantity cannot be waried from widget to widget!
  switch(align)
    {
    default:
    case Qt::AlignLeft:
      _width=maxSize;
      break;
    case Qt::AlignTop:
      _height=maxSize;
      break;
    }

  
  recalculateAlignment();
  recalculatePositions();
}

int ASlidyManager::visibleId()
{
  return _visibleId;
}

void ASlidyManager::setPosition(int position)
{
  _position=position;
  
  recalculateAlignment();
  recalculatePositions();
}

void ASlidyManager::setWidth(int width)
{
  _width=width;

  recalculateAlignment();
  recalculateSize();
}

void ASlidyManager::setHeight(int height)
{
  _height=height;

  recalculateAlignment();
  recalculateSize();
}

int ASlidyManager::addWidget(QWidget *wdg,QString title,bool controlable)
{
  // Create the flaps
  ASlidyWidget *flap=new ASlidyWidget(this,title,(QWidget*)parent(),controlable);

  int width, height;
  if(_width>0) width=_width;
  else width=wdg->width();

  if(_height>0) height=_height;
  else height=wdg->height();

  flap->resize(width,height);
  flap->addWidget(wdg);

  _flaps.push_back(flap);

  recalculatePositions();

  QString name;
  if(title.isEmpty())
    name=wdg->objectName()+"_SlidyFrame";
  else
    {
      name=QString(title);
      name.replace(" ","_");
    }
  flap->setObjectName(name);

  return _flaps.size()-1;
}

bool ASlidyManager::eventFilter(QObject *watched,QEvent *event)
{
  if(event->type()==QEvent::Resize)
    {
      QResizeEvent *resizeEvent=(QResizeEvent*)event;
      QSize parentSize=resizeEvent->size();

      recalculateAlignment();
      recalculatePositions();
    }
  return false;
}

void ASlidyManager::toggleWidget(int id)
{
  if(_visibleId==id)
    hideWidget(id);
  else
    showWidget(id);
}

void ASlidyManager::showWidgetTimed(int id)
{
  bool startTimer=((_visibleId==id && timer.isActive())
		   || _visibleId!=id);
  showWidget(id);
  
  if(startTimer) timer.start();
}

void ASlidyManager::showWidget(int id)
{
  if(!_flaps[id]->isVisible()) _flaps[id]->show();
  
  if(_visibleId==id) return;
  
  showingAnimator.setWidget(_flaps[id]);

  if(_visibleId!=-1)
    {
      hidingAnimator.setWidget(_flaps[_visibleId]);
    }
  else
    hidingAnimator.setWidget(0);
  
  _flaps[id]->raise();
  
  mainTimeline.start();
  _lastVisibleId=_visibleId;
  _visibleId=id;

  timer.stop();
}

void ASlidyManager::hideWidget()
{
  if(_visibleId!=-1)
    hideWidget(_visibleId);
}

void ASlidyManager::hideWidget(int id)
{
  if(_visibleId!=id) return;

  showingAnimator.setWidget(0);
  hidingAnimator.setWidget(_flaps[id]);
  
  mainTimeline.start();
  _lastVisibleId=_visibleId;
  _visibleId=-1;
  timer.stop();
}

void ASlidyManager::recalculatePositions()
{
  QPoint pos=topCorner;
  for(int i=0;i<_flaps.size();i++)
    {
      if(_visibleId==i)
	{
	  QPoint visiblePos=pos+moveIn;
	  _flaps[i]->move(visiblePos);
	}
      else
	_flaps[i]->move(pos);
    }
}

void ASlidyManager::recalculateAlignment()
{
  QSize parentSize=((QWidget*)parent())->size();
  
  switch(align)
    {
    default:
    case Qt::AlignLeft:
      verticalSlide=false;
      moveIn=QPoint(-_width,0);
      topCorner=QPoint(parentSize.width(),_position);
      break;
    case Qt::AlignTop:
      verticalSlide=true;
      moveIn=QPoint(0,_height);
      topCorner=QPoint(_position,-_height);
      break;
    }

  prepareShowAnimation();
  prepareHideAnimation();
}

void ASlidyManager::recalculateSize()
{
  QList<ASlidyWidget *>::const_iterator iter=_flaps.begin();
  QList<ASlidyWidget *>::const_iterator iterE=_flaps.end();
  for(;iter!=iterE;++iter)
    {
      int width, height;
      if(_width>0) width=_width;
      else width=(*iter)->width();
      
      if(_height>0) height=_height;
      else height=(*iter)->height();
      
      (*iter)->resize(width,height);
    }
}

void ASlidyManager::timerEvent()
{
  if(_visibleId!=-1) hideWidget(_visibleId);
}

void ASlidyManager::prepareShowAnimation()
{
  showingAnimator.setPosAt(0,topCorner);
  showingAnimator.setPosAt(1,topCorner+moveIn);
}

void ASlidyManager::prepareHideAnimation()
{
  hidingAnimator.setPosAt(0,topCorner);
  hidingAnimator.setPosAt(1,topCorner+moveIn);
}
