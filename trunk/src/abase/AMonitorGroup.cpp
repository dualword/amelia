#include "AMonitorGroup.h"
#include <QDebug>
#include <math.h>

AMonitorGroup::AMonitorGroup(QString _name,QGraphicsItem* _parent):QGraphicsItemGroup(_parent),name(_name)
{ 
  scale(0.25,0.25);
  setHandlesChildEvents(false); //Let the monitors handle it's own clicks
  setZValue(100);
  setVisible(false); //Do not show until we call this in action!
  setPos(-1000,-1000); //Hide it somewhere faar far away. This avoids the initial flicker.

  //This prepares animation & timers!
  dropDownTime.setDuration(1000);
  dropDownTime.setCurveShape(QTimeLine::EaseOutCurve);
  dropDown.setTimeLine(&dropDownTime);
  dropDown.setItem(this);
  pullUpTime.setDuration(1000);
  pullUpTime.setCurveShape(QTimeLine::EaseOutCurve);
  pullUp.setTimeLine(&pullUpTime);
  pullUp.setItem(this);
  updateAnimatorPositions();
}

void AMonitorGroup::addMonitor(QString _name,AMonitor *monitor)
{ 
  monitor->setParentItem(this);
  monitor->setPos(1024*(_monitors.size()),0);
  _monitors[_name]=monitor;

  if(isVisible()) setPos(calculateScaledWidgetGroupPosition());
  
  updateAnimatorPositions();
  qDebug() << "Added " << _name << " to " << name;
}

QMap<QString,AMonitor*> AMonitorGroup::monitors()
{
  return _monitors;
}

AMonitor* AMonitorGroup::monitor(QString _name)
{
  return _monitors[_name];
}

void AMonitorGroup::show()
{ 
  pullUpTime.stop();
  dropDownTime.start();
  setVisible(true);
}

void AMonitorGroup::hide()
{ 
  pullUpTime.start();
  dropDownTime.stop();
}

QPointF AMonitorGroup::calculateScaledWidgetGroupPosition()
{
  if (_monitors.size()==0) return QPoint();
  
  //Figures out where the widget group would go if it were scaled by 0.2
  // and centered
  QPointF ret= QPointF((1024-childrenBoundingRect().width()*0.25)/2,
		       (768-childrenBoundingRect().height()*0.25)/2 - 150);
  return ret;
}

void AMonitorGroup::updateAnimatorPositions()
{
  QPointF visibleAt=calculateScaledWidgetGroupPosition();
  QPointF hiddenAt=visibleAt;
  hiddenAt.setY(-childrenBoundingRect().height()*0.25-5);

  qreal h=visibleAt.y()-hiddenAt.y();
  for(qreal i=0;i<=1000;i++)
    {
      qreal t=i/1000;
      qreal xDown=exp(-t/0.15)*cos(2*3.14*2.25*t);
      qreal xUp=1-exp(-t/0.15)*cos(2*3.14*0.75*t);
      xDown*=h;
      xUp*=h;
      dropDown.setPosAt(t,QPoint(visibleAt.x(),visibleAt.y()-(int)xDown));
      pullUp.setPosAt(t,QPoint(visibleAt.x(),visibleAt.y()-(int)xUp));
    }
}
