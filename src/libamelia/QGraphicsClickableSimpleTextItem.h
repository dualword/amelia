#ifndef QGRAPHICSCLICABLEPIXMAPITEM_H_
#define QGRAPHICSCLICABLEPIXMAPITEM_H_

#include <QGraphicsSimpleTextItem>
#include <QObject>
#include <QTimeLine>

#include "ADefines.h"

class LIBAMELIA_EXPORT QGraphicsClickableSimpleTextItem : public QObject,public QGraphicsSimpleTextItem
{
    Q_OBJECT
public:
    QGraphicsClickableSimpleTextItem( QGraphicsItem * parent = 0 );
    QGraphicsClickableSimpleTextItem( const QString & text, QGraphicsItem * parent = 0 );

signals:
    void clicked();
    void mouseEnter();
    void mouseLeave();

protected:
    virtual void mousePressEvent( QGraphicsSceneMouseEvent *event );
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
};

#endif //QGRAPHICSCLICABLEPIXMAPITEM_H_

