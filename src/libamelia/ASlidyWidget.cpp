#include "ASlidyWidget.h"
#include <QDebug>
#include <QResizeEvent>
#include <QStyleOption>
#include <QPainter>
#include <math.h>

ASlidyWidget::ASlidyWidget(QWidget *parent)
  :QObject(parent),mainTimeline(1000),
   showingTimeline(&mainTimeline,&mainTimeline),
   hidingTimeline(&mainTimeline,&mainTimeline),
   _visibleId(-1),_lastVisibleId(-1),_maxWidth(250)
{ 
  parent->installEventFilter(this);
  
  connect(&flapMapper,SIGNAL(mapped(int)),
	  this,SLOT(toggleWidget(int)));

  showingAnimator.setTimeLine(&showingTimeline);
  hidingAnimator.setTimeLine(&hidingTimeline);
  hidingTimeline.setDirection(QTimeLine::Backward);

  mainTimeline.setCurveShape(QTimeLine::LinearCurve);
  connect(&mainTimeline,SIGNAL(valueChanged(qreal)),
	  this,SLOT(raiseFlap(qreal)));

  timer.setSingleShot(true);
  timer.setInterval(10000);
  connect(&timer,SIGNAL(timeout()),
	  this,SLOT(timerEvent()));
}

int ASlidyWidget::addWidget(QWidget *wdg,QString title)
{
  // Create the flaps
  ASlidyFlap *flap=new ASlidyFlap((QWidget*)parent());
  flap->setText(title);
  flap->setCheckable(true);

  connect(flap,SIGNAL(clicked()),
	  &flapMapper,SLOT(map()));
  flapMapper.setMapping(flap,_pages.size());

  flap->attachWidget(wdg);
  wdg->setParent((QWidget*)parent());
  wdg->move(flap->width(),0);
  wdg->resize(_maxWidth-20,wdg->height());

  _pages.push_back(wdg);
  _flaps.push_back(flap);

  recalculatePositions();

  return _pages.size()-1;
}

bool ASlidyWidget::eventFilter(QObject *watched,QEvent *event)
{
  if(event->type()==QEvent::Resize)
    {
      QResizeEvent *resizeEvent=(QResizeEvent*)event;
      QSize parentSize=resizeEvent->size();
      topCorner=QPoint(parentSize.width()-15,20);
      recalculatePositions();
    }
  return false;
}

void ASlidyWidget::toggleWidget(int id)
{
  if(_visibleId==id)
    hideWidget(id);
  else
    showWidget(id);
}

void ASlidyWidget::showWidgetTimed(int id)
{
  showWidget(id);
  timer.start();
}

void ASlidyWidget::showWidget(int id)
{
  if(_visibleId==id) return;
  
  prepareShowAnimation(_flaps[id]);
  showingAnimator.setWidget(_flaps[id]);
  
  if(_visibleId!=-1)
    {
      prepareHideAnimation(_flaps[_visibleId]);
      hidingAnimator.setWidget(_flaps[_visibleId]);
      _pages[_visibleId]->raise();
    }
  else
    hidingAnimator.setWidget(0);

  _pages[id]->raise();
  for(int i=0;i<_flaps.size();i++)
    if(i!=_visibleId)
      _flaps[i]->raise();
  
  mainTimeline.start();
  _lastVisibleId=_visibleId;
  _visibleId=id;

  timer.stop();
}

void ASlidyWidget::hideWidget(int id)
{
  if(_visibleId!=id) return;

  prepareHideAnimation(_flaps[id]);
  showingAnimator.setWidget(0);
  hidingAnimator.setWidget(_flaps[id]);
  
  mainTimeline.start();
  _lastVisibleId=_visibleId;
  _visibleId=-1;
  timer.stop();
}

void ASlidyWidget::recalculatePositions()
{
  QPoint pos=topCorner;
  for(int i=0;i<_pages.size();i++)
    {
      if(_visibleId==i)
	{
	  QPoint visiblePos=pos;
	  visiblePos.setX(visiblePos.x()-_maxWidth);
	  _flaps[i]->move(visiblePos);
	}
      else
	_flaps[i]->move(pos);
      pos+=QPoint(0,_flaps[i]->size().height()+5);
    }
}

void ASlidyWidget::raiseFlap(qreal time)
{
  if(_lastVisibleId==-1) return;

  QWidget *parent=(QWidget*)this->parent();
  if(parent->width()<_flaps[_lastVisibleId]->x())
    _flaps[_lastVisibleId]->raise();
}

void ASlidyWidget::timerEvent()
{
  qDebug() << "TIMER";
  if(_visibleId!=-1) hideWidget(_visibleId);
}

void ASlidyWidget::prepareShowAnimation(ASlidyFlap *flap)
{
  //Calculate direction
  for(qreal t=0;t<=1;t+=0.1)
    {
      int xpos=_maxWidth*sin(2./4.*M_PI*t);
      QPoint pos1=topCorner;
      pos1.setX(pos1.x()-xpos);

      pos1.setY(flap->y());
      showingAnimator.setPosAt(t,pos1);
    }
}

void ASlidyWidget::prepareHideAnimation(ASlidyFlap *flap)
{
  //Calculate direction
  for(qreal t=0;t<=1;t+=0.1)
    {
      int xpos=_maxWidth*sin(6./4.*M_PI*t);
      QPoint pos1=topCorner;
      pos1.setX(pos1.x()+xpos);

      pos1.setY(flap->y());
      hidingAnimator.setPosAt(t,pos1);
    }
}
