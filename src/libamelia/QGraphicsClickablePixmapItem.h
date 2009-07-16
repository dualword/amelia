#ifndef QGRAPHICSCLICABLEPIXMAPITEM_H_
#define QGRAPHICSCLICABLEPIXMAPITEM_H_

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimeLine>

class QGraphicsClickablePixmapItem : public QObject,public QGraphicsPixmapItem
{
public:
  QGraphicsClickablePixmapItem( QGraphicsItem * parent = 0 );
  QGraphicsClickablePixmapItem( const QPixmap & pixmap, QGraphicsItem * parent = 0 );
  
signals:
  void clicked();
  void mouseEnter();
  void mouseLeave();
  
protected:
  virtual void mousePressEvent( QGraphicsSceneMouseEvent *event );
  void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
  void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
  
  Q_OBJECT
};

#endif //QGRAPHICSCLICABLEPIXMAPITEM_H_

