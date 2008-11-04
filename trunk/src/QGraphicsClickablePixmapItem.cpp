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
