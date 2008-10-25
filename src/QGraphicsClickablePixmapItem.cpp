#include "QGraphicsClickablePixmapItem.h"

QGraphicsClickablePixmapItem::QGraphicsClickablePixmapItem( QGraphicsItem * parent ):QGraphicsPixmapItem(parent) { }

QGraphicsClickablePixmapItem::QGraphicsClickablePixmapItem( const QPixmap & pixmap, QGraphicsItem * parent ):QGraphicsPixmapItem(pixmap,parent) { }

void QGraphicsClickablePixmapItem::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
  emit clicked();
}
