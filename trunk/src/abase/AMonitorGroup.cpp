#include "AMonitorGroup.h"
#include <QDebug>
#include <math.h>


AMonitorGroup::AMonitorGroup(QGraphicsItem* _parent)
 :QGraphicsItemLayout(_parent)
{ 
  scale(0.25,0.25);
  setHandlesChildEvents(false); //Let the monitors handle it's own clicks
  setZValue(100);
  setVisible(false); //Do not show until we call this in action!
  setPos(-1000,-1000); //Hide it somewhere faar far away. This avoids the initial flicker.
  setHorizontalAlignment(Qt::AlignHCenter);

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

void AMonitorGroup::addMonitor(QString _name,AMonitor *monitor,Qt::Alignment align)
{ 
  monitor->setParentItem(this);
  addToGroup(monitor,align);
  _monitors[_name]=monitor;

  updateAnimatorPositions();
  qDebug() << "Added " << _name;
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

void AMonitorGroup::setSelected(const QString& selected)
{
  _selected=selected;
  calculatePositions();
}

QPointF AMonitorGroup::calculateScaledWidgetGroupPosition()
{
  QPointF ret= QPointF(1024/2,
		       150);
  return ret;
}

void AMonitorGroup::updateAnimatorPositions()
{
  QPointF visibleAt=calculateScaledWidgetGroupPosition();
  QPointF hiddenAt=visibleAt;
  hiddenAt.setY(-childrenBoundingRect().height()*0.40-5);

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

QTransform AMonitorGroup::calculateTransformationForItem(int idx)
{
  float rotationAngle=10;
  
  QTransform trans;
  
  QList<QGraphicsItem *> items=this->items();
  int centerIdx;
  if(_selected.isEmpty())
    centerIdx=items.size()/2;
  else
    for(int i=0;i<items.size();i++)
      if(_monitors.key((AMonitor*)items[i])==_selected)
	centerIdx=i;
  
  QRectF itemRect=items[idx]->boundingRect();

  if(items.size()%2==1 || !_selected.isEmpty()) //Rotate if there is no central monitor
    trans.translate(-items[centerIdx]->boundingRect().width()/2,0);
  else
    trans.translate(5,0);

  if(idx>=centerIdx)
    { //This monitor is to the right of center
      if(items.size()%2==0 && _selected.isEmpty()) //Rotate if there is no central monitor
	trans.rotate(rotationAngle,Qt::YAxis);

      for(int i=centerIdx;i<idx;i++)
	{
	  QRectF rect=items[i]->boundingRect();
	  trans.translate(rect.width()+10,0);
	  trans.rotate(rotationAngle,Qt::YAxis);
	}
    }
  else if(idx<centerIdx)
    { //This monitor to the left of center
      for(int i=centerIdx;i>idx;i--)
	{
	  QRectF rect=items[i]->boundingRect();
	  trans.translate(-rect.width()-10,0);
	  
	  trans.translate(itemRect.width(),0);
	  trans.rotate(-rotationAngle,Qt::YAxis);
	  trans.translate(-itemRect.width(),0);
	}
    }

  return trans;
}
