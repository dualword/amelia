#include "QGraphicsClickablePixmapItem.h"
#include <QDebug>

QGraphicsClickablePixmapItem::QGraphicsClickablePixmapItem( QGraphicsItem * parent ):QGraphicsPixmapItem(parent)
{
    setAcceptHoverEvents(true);
    animDirection = 1;
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

void QGraphicsClickablePixmapItem::setMenuAnimationDirectionForward()
{
    animDirection = 1;
}
void QGraphicsClickablePixmapItem::setMenuAnimationDirectionBackward()
{
    animDirection = -1;
}

void QGraphicsClickablePixmapItem::itemTransformStep(qreal step)
{
    QTransform t;
    t.translate(512 , 0);
    float yrot = 0;
    if (animDirection == 1)
        yrot = (1-step)*yRot;

    if (animDirection == -1)
        yrot = step*yRot;
    float xtrans = xTrans*(1 -menuPosition)*step;
    t.rotate(yrot, Qt::YAxis);
    t.translate(-512 , 0);
    this->setTransform(t);
}
