#include "ASlidyManager.h"
#include <QDebug>
#include <QResizeEvent>
#include <QStyleOption>
#include <QPainter>
#include <math.h>

ASlidyManager::ASlidyManager(QWidget *parent,Qt::Alignment align)
  :QObject(parent),mainTimeline(1000),
   showingTimeline(&mainTimeline,&mainTimeline),
   hidingTimeline(&mainTimeline,&mainTimeline),
   _visibleId(-1),_lastVisibleId(-1),_maxSize(250),
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

  
  recalculateAlignment();
  prepareShowAnimation();
  prepareHideAnimation();
  recalculatePositions();
}

void ASlidyManager::setMaxSize(int maxSize)
{
  _maxSize=maxSize;

  recalculateAlignment();
}

int ASlidyManager::addWidget(QWidget *wdg,QString title)
{
  // Create the flaps
  ASlidyWidget *flap=new ASlidyWidget((QWidget*)parent());

  if(verticalSlide)
    wdg->resize(wdg->size().width(),_maxSize);
  else
    wdg->resize(_maxSize,wdg->size().height());

  flap->addWidget(wdg);
  _flaps.push_back(flap);

  recalculatePositions();

  return _flaps.size()-1;
}

bool ASlidyManager::eventFilter(QObject *watched,QEvent *event)
{
  if(event->type()==QEvent::Resize)
    {
      QResizeEvent *resizeEvent=(QResizeEvent*)event;
      QSize parentSize=resizeEvent->size();

      switch(align)
	{
	default:
	case Qt::AlignLeft:
	  topCorner=QPoint(parentSize.width(),100);
	  break;
	case Qt::AlignTop:
	  topCorner=QPoint(50,-_maxSize);
	  break;
	}
      prepareShowAnimation();
      prepareHideAnimation();
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
      moveIn=QPoint(-_maxSize,0);
      topCorner=QPoint(parentSize.width(),100);
      break;
    case Qt::AlignTop:
      verticalSlide=true;
      moveIn=QPoint(0,_maxSize);
      topCorner=QPoint(50,-_maxSize);
      break;
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
