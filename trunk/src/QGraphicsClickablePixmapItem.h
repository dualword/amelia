#ifndef QGRAPHICSCLICABLEPIXMAPITEM_H_
#define QGRAPHICSCLICABLEPIXMAPITEM_H_

#include <QGraphicsPixmapItem>
#include <QObject>

class QGraphicsClickablePixmapItem : public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    QGraphicsClickablePixmapItem( QGraphicsItem * parent = 0 );
    QGraphicsClickablePixmapItem( const QPixmap & pixmap, QGraphicsItem * parent = 0 );
    float yRot;
    float xTrans;
    int menuPosition;

public slots:
    void itemTransformStep(qreal step);

signals:
    void clicked();
    void mouseEnter();
    void mouseLeave();

protected:
    virtual void mousePressEvent( QGraphicsSceneMouseEvent *event );
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    bool isFormatBig;
};

#endif //QGRAPHICSCLICABLEPIXMAPITEM_H_

