#include "QGraphicsItemLayout.h"
#include <QDebug>
#include <QTimeLine>

QGraphicsItemLayout::QGraphicsItemLayout(QGraphicsItem *parent)
  :QGraphicsItemGroup(parent),_orientation(Qt::Horizontal),
   _verticalAlignment(Qt::AlignTop),_horizontalAlignment(Qt::AlignLeft)
{ }

QGraphicsItemLayout::QGraphicsItemLayout(Qt::Orientation orient,QGraphicsItem *parent)
  :QGraphicsItemGroup(parent),_orientation(orient),
   _verticalAlignment(Qt::AlignTop),_horizontalAlignment(Qt::AlignLeft)
{ }


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

void QGraphicsItemLayout::setOrientation(Qt::Orientation orient)
{
  _orientation=orient;
  calculatePositions();
}

void QGraphicsItemLayout::addToGroup(QGraphicsItem *item,Qt::Alignment align)
{
  QGraphicsItemAnimation *animator=new QGraphicsItemAnimation();
  QTimeLine *timer=new QTimeLine(1000);

  animator->setItem(item);
  animator->setTimeLine(timer);

  if(align==Qt::AlignLeft)
    {
      animations.push_front(animator);
      items.push_front(item);
    }
  else if(align==Qt::AlignRight)
    {
      animations.push_back(animator);
      items.push_back(item);
    }
  else
    {
      int idx=items.size()/2;
      animations.insert(idx,animator);
      items.insert(idx,item);
    }

  QGraphicsItemGroup::addToGroup(item);
  calculatePositions();
}

void QGraphicsItemLayout::calculatePositions()
{
  float padding=10;

  float spaceFullTakenUpInX=0;
  float spaceFullTakenUpInY=0;
  // Calculate the dimensions of the layout
  for(int i=0;i<items.size();i++)
    {
      QGraphicsItem *item=items[i];
      QRectF rect=item->boundingRect();
      if(_orientation==Qt::Horizontal)
	{
	  spaceFullTakenUpInX+=rect.width()+padding;
	  if(spaceFullTakenUpInY<rect.height()) spaceFullTakenUpInY=rect.height();
	}
      else
	{
	  spaceFullTakenUpInY+=rect.height()+padding;
	  if(spaceFullTakenUpInX<rect.width()) spaceFullTakenUpInX=rect.width();
	}
    }
  // Do the actual moves. Yaay
  float spaceTakenUpInX=0;
  float spaceTakenUpInY=0;
  for(int i=0;i<items.size();i++)
    {
      QGraphicsItem *item=items[i];
      QPointF pos=item->pos();
      animations[i]->setPosAt(0,pos);

      QRectF rect=item->boundingRect();
      
      switch(_horizontalAlignment)
	{
	case Qt::AlignLeft:
	  pos.setX(spaceTakenUpInX);
	  break;
	case Qt::AlignHCenter:
	  pos.setX(spaceTakenUpInX-spaceFullTakenUpInX/2);
	  break;
	case Qt::AlignRight:
	  pos.setX(spaceTakenUpInX-spaceFullTakenUpInX);
	  break;
	}	  

      switch(_verticalAlignment)
	{
	case Qt::AlignTop:
	  pos.setY(spaceTakenUpInY);
	  break;
	case Qt::AlignVCenter:
	  pos.setY(spaceTakenUpInY-spaceFullTakenUpInY/2);
	  break;
	case Qt::AlignBottom:
	  pos.setY(spaceTakenUpInY-spaceFullTakenUpInY);
	  break;
	}

      if(_orientation==Qt::Horizontal)
	{
	  spaceTakenUpInX+=rect.width()+padding;
	}
      else
	{
	  spaceTakenUpInY+=rect.height()+padding;
	}
	  
      animations[i]->setPosAt(1,pos);
      animations[i]->timeLine()->start();
    }

  // Move the menu, if anchored
  /*if(anchored)
    {
      anchorAnimator.setPosAt(0,pos());
      QPointF finalPos=_anchorPos;
      anchorAnimator.setPosAt(1,finalPos);
      anchorAnimator.timeLine()->start();
      }*/
}
