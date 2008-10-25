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

signals:
  void clicked();

protected:
  virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );
};

#endif //QGRAPHICSCLICABLEPIXMAPITEM_H_
