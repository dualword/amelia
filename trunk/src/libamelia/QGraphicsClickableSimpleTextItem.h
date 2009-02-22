#ifndef QGRAPHICSCLICABLEPIXMAPITEM_H_
#define QGRAPHICSCLICABLEPIXMAPITEM_H_

#include <QGraphicsSimpleTextItem>
#include <QObject>
#include <QTimeLine>

#include "ADefines.h"

class QGraphicsClickableSimpleTextItem : public QObject,public QGraphicsSimpleTextItem
{
    Q_OBJECT
public:
    Q_GUI_EXPORT QGraphicsClickableSimpleTextItem( QGraphicsItem * parent = 0 );
    Q_GUI_EXPORT QGraphicsClickableSimpleTextItem( const QString & text, QGraphicsItem * parent = 0 );

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

