#include "QGraphicsItemTransformAnimation.h"
#include <QDebug>

QGraphicsItemTransformAnimation::QGraphicsItemTransformAnimation(QObject *parent)
  :QObject(parent),_timeLine(0),_item(0)
{ }

QTimeLine* QGraphicsItemTransformAnimation::timeLine()
{ return _timeLine; }

void QGraphicsItemTransformAnimation::setTimeLine(QTimeLine *timeLine)
{
  if(_timeLine!=0)
    disconnect(_timeLine,SIGNAL(valueChanged(qreal)),
	       this,SLOT(setStep(qreal)));

  _timeLine=timeLine;
  connect(_timeLine,SIGNAL(valueChanged(qreal)),
	  this,SLOT(setStep(qreal)));
}

QGraphicsItem *QGraphicsItemTransformAnimation::item()
{ return _item; }

void QGraphicsItemTransformAnimation::setItem(QGraphicsItem *item)
{ _item=item; }

QList<QPair<qreal,QTransform> > QGraphicsItemTransformAnimation::transformList()
{ return _transformList; }

void QGraphicsItemTransformAnimation::setTransformAt(qreal step,QTransform transform)
{ 
  QPair<qreal,QTransform> entry(step,transform);
  //Insert in order..
  for(int i=0;i<_transformList.size();i++)
    {
      qreal curStep=_transformList[i].first;
      if(step<curStep)
	{
	  _transformList.insert(i,entry);
	  return;
	}
      else if(step==curStep)
	{
	  _transformList[i]=entry;
	  return;
	}
    }
  
  _transformList.append(entry);
}

void QGraphicsItemTransformAnimation::setStep(qreal step)
{
  if(_item==0) return;
  if(_transformList.size()==0) return;

  QTransform transform=calculateTransform(0,_transformList.size()-1,step);

  _item->setTransform(transform);
}

QTransform QGraphicsItemTransformAnimation::calculateTransform(int start,int end,qreal step)
{
  if(end-start==1)
    { //Found it!
      qreal stepNext=_transformList[end].first;
      qreal stepPrev=_transformList[start].first;
      qreal stepDelta=(stepPrev-step)/(stepNext-stepPrev);
      
      QTransform transformNext=_transformList[end].second;
      QTransform transformPrev=_transformList[start].second;
      qreal m11Delta=transformNext.m11()-transformPrev.m11();
      qreal m12Delta=transformNext.m12()-transformPrev.m12();
      qreal m13Delta=transformNext.m13()-transformPrev.m13();
      qreal m21Delta=transformNext.m21()-transformPrev.m21();
      qreal m22Delta=transformNext.m22()-transformPrev.m22();
      qreal m23Delta=transformNext.m23()-transformPrev.m23();
      qreal m31Delta=transformNext.m31()-transformPrev.m31();
      qreal m32Delta=transformNext.m32()-transformPrev.m32();
      qreal m33Delta=transformNext.m33()-transformPrev.m33();
      m11Delta*=stepDelta;
      m12Delta*=stepDelta;
      m13Delta*=stepDelta;
      m21Delta*=stepDelta;
      m22Delta*=stepDelta;
      m23Delta*=stepDelta;
      m31Delta*=stepDelta;
      m32Delta*=stepDelta;
      m33Delta*=stepDelta;

      QTransform transform(transformPrev.m11()+m11Delta*stepDelta,transformPrev.m12()+m12Delta*stepDelta,transformPrev.m13()+m13Delta*stepDelta,
			   transformPrev.m21()+m21Delta*stepDelta,transformPrev.m22()+m22Delta*stepDelta,transformPrev.m23()+m23Delta*stepDelta,
			   transformPrev.m31()+m31Delta*stepDelta,transformPrev.m32()+m32Delta*stepDelta,transformPrev.m33()+m33Delta*stepDelta);
      return transform;
    }
  
  int at=(end+start)/2;
  
  qreal stepAt=_transformList[at].first;
  if(stepAt==step || at==0)
    {
      return _transformList[at].second;
    }
  else if(step<stepAt)
    {
      return calculateTransform(start,at,step);
    }
  else if(step>stepAt)
    {
      return calculateTransform(at,end,step);
    }

  return QTransform();
}
