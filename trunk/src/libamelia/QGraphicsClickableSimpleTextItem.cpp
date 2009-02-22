#include "QGraphicsClickableSimpleTextItem.h"
#include <QDebug>
#include <QCursor>

QGraphicsClickableSimpleTextItem::QGraphicsClickableSimpleTextItem( QGraphicsItem * parent ):QGraphicsSimpleTextItem(parent)
{
  setAcceptHoverEvents(true);
  setCursor(QCursor(Qt::PointingHandCursor));
}

QGraphicsClickableSimpleTextItem::QGraphicsClickableSimpleTextItem( const QString & text, QGraphicsItem * parent ):QGraphicsSimpleTextItem(text,parent) 
{ 
  setAcceptHoverEvents(true);
  setCursor(QCursor(Qt::PointingHandCursor));
}

void QGraphicsClickableSimpleTextItem::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
  emit clicked();
}

void QGraphicsClickableSimpleTextItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
  emit mouseEnter();
}

void QGraphicsClickableSimpleTextItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
  emit mouseLeave();
}
