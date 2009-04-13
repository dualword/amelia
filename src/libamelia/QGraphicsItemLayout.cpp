#include "QGraphicsItemLayout.h"
#include <QDebug>
#include <QTimeLine>

QGraphicsItemLayout::QGraphicsItemLayout(QGraphicsItem *parent)
  :QGraphicsItemGroup(parent),_orientation(Qt::Horizontal),
   _verticalAlignment(Qt::AlignTop),_horizontalAlignment(Qt::AlignLeft),
   _fullSpaceTakenUpInX(0),_fullSpaceTakenUpInY(0),_padding(10),
   _timeLine(new QTimeLine(1000))
{ 
  connect(_timeLine,SIGNAL(finished()),
	  this,SLOT(handleAnimationFinished()));
}

QGraphicsItemLayout::QGraphicsItemLayout(Qt::Orientation orient,QGraphicsItem *parent)
  :QGraphicsItemGroup(parent),_orientation(orient),
   _verticalAlignment(Qt::AlignTop),_horizontalAlignment(Qt::AlignLeft),
   _fullSpaceTakenUpInX(0),_fullSpaceTakenUpInY(0),_padding(10),
   _timeLine(new QTimeLine(1000))
{
  connect(_timeLine,SIGNAL(finished()),
	  this,SLOT(handleAnimationFinished()));
}


Qt::Alignment QGraphicsItemLayout::verticalAlignment()
{
  return _verticalAlignment;
}

void QGraphicsItemLayout::setVerticalAlignment(Qt::Alignment align)
{
  _verticalAlignment=align;
  calculatePositions();
}

Qt::Alignment QGraphicsItemLayout::horizontalAlignment()
{
  return _horizontalAlignment;
}

void QGraphicsItemLayout::setHorizontalAlignment(Qt::Alignment align)
{
  _horizontalAlignment=align;
  calculatePositions();
}

Qt::Orientation QGraphicsItemLayout::orientation()
{
  return _orientation;
}

QTimeLine *QGraphicsItemLayout::timeLine()
{
  return _timeLine;
}

void QGraphicsItemLayout::setOrientation(Qt::Orientation orient)
{
  _orientation=orient;
  calculatePositions();
}

float QGraphicsItemLayout::fullSpaceTakenUpInX()
{
  return _fullSpaceTakenUpInX;
}

float QGraphicsItemLayout::fullSpaceTakenUpInY()
{
  return _fullSpaceTakenUpInY;
}

QList<QGraphicsItem *> QGraphicsItemLayout::items()
{
  return _items;
}

void QGraphicsItemLayout::handleAnimationFinished()
{
  emit layoutReady();
}

void QGraphicsItemLayout::addToGroup(QGraphicsItem *item,Qt::Alignment align)
{
  QGraphicsItemTransformAnimation *animator=new QGraphicsItemTransformAnimation();

  item->setParentItem(this);
  animator->setItem(item);
  animator->setTimeLine(_timeLine);

  if(align==Qt::AlignLeft)
    {
      animations.push_front(animator);
      _items.push_front(item);
    }
  else if(align==Qt::AlignRight)
    {
      animations.push_back(animator);
      _items.push_back(item);
    }
  else
    {
      int idx=_items.size()/2;
      animations.insert(idx,animator);
      _items.insert(idx,item);
    }

  QGraphicsItemGroup::addToGroup(item);
  calculatePositions();
}

void QGraphicsItemLayout::calculatePositions()
{
  if(_timeLine->state()==QTimeLine::Running)
    _timeLine->stop();

  _fullSpaceTakenUpInX=0;
  _fullSpaceTakenUpInY=0;
  // Calculate the dimensions of the layout
  for(int i=0;i<_items.size();i++)
    {
      QGraphicsItem *item=_items[i];
      QRectF rect=item->boundingRect();
      if(_orientation==Qt::Horizontal)
	{
	  _fullSpaceTakenUpInX+=rect.width()+_padding;
	  if(_fullSpaceTakenUpInY<rect.height()) _fullSpaceTakenUpInY=rect.height();
	}
      else
	{
	  _fullSpaceTakenUpInY+=rect.height()+_padding;
	  if(_fullSpaceTakenUpInX<rect.width()) _fullSpaceTakenUpInX=rect.width();
	}
    }
  
  // Do the actual moves. Yaay
  for(int i=0;i<_items.size();i++)
    {
      QTransform trans=calculateTranslationForItem(i);
      animations[i]->setTransformAt(0,_items[i]->transform());
      animations[i]->setTransformAt(1,trans);
    }

  _timeLine->start();
}

QTransform QGraphicsItemLayout::calculateTranslationForItem(int idx)
{
  float spaceTakenUpInX=0;
  float spaceTakenUpInY=0;
  for(int i=0;i<idx;i++)
    {
      QGraphicsItem *item=_items[i];

      QRectF rect=item->boundingRect();
      
      if(_orientation==Qt::Horizontal)
	spaceTakenUpInX+=rect.width()+_padding;
      else
	spaceTakenUpInY+=rect.height()+_padding;
    }      

  QTransform trans=QTransform::fromTranslate(spaceTakenUpInX,spaceTakenUpInY);
  
  switch(_horizontalAlignment)
    {
    case Qt::AlignLeft:
      trans.translate(0,0);
      break;
    case Qt::AlignHCenter:
      trans.translate(-_fullSpaceTakenUpInX/2,0);
      break;
    case Qt::AlignRight:
      trans.translate(-_fullSpaceTakenUpInX,0);
      break;
    }	  
  
  switch(_verticalAlignment)
    {
    case Qt::AlignTop:
      trans.translate(0,0);
      break;
    case Qt::AlignVCenter:
      trans.translate(0,-_fullSpaceTakenUpInY/2);
      break;
    case Qt::AlignBottom:
      trans.translate(0,-_fullSpaceTakenUpInY);
      break;
    }

  return trans;
}
