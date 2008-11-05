#include "QGraphicsClickablePixmapItem.h"
#include <QDebug>

QGraphicsClickablePixmapItem::QGraphicsClickablePixmapItem( QGraphicsItem * parent ):QGraphicsPixmapItem(parent)
{
  setAcceptHoverEvents(true);
}

QGraphicsClickablePixmapItem::QGraphicsClickablePixmapItem( const QPixmap & pixmap, QGraphicsItem * parent ):QGraphicsPixmapItem(pixmap,parent) { }

void QGraphicsClickablePixmapItem::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
  emit clicked();
}

void QGraphicsClickablePixmapItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
  emit mouseEnter();
}

void QGraphicsClickablePixmapItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
  emit mouseLeave();
}

void QGraphicsClickablePixmapItem::itemTransformStep(qreal step)
{
    QTransform t;
    t.translate(512 , 0);
    float yrot = step*yRot;
    float xtrans = xTrans*(1 -menuPosition)*step;
    t.rotate(yRot - yrot, Qt::YAxis);
    t.translate(-512 , 0);
    this->setTransform(t);
}
