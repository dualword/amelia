#ifndef QGRAPHICSITEMLAYOUT_H_
#define QGRAPHICSITEMLAYOUT_H_

#include <QGraphicsItemGroup>
#include <QGraphicsItemAnimation>
#include <QGraphicsRectItem>
#include <QObject>

class QGraphicsItemLayout : public QGraphicsItemGroup
{
  
public:
  QGraphicsItemLayout(QGraphicsItem *parent=0);
  QGraphicsItemLayout(Qt::Orientation orient, QGraphicsItem *parent=0);

  void setVerticalAlignment(Qt::Alignment align);
  Qt::Alignment verticalAlignment();
  void setHorizontalAlignment(Qt::Alignment align);
  Qt::Alignment horizontalAlignment();
  
  void addToGroup(QGraphicsItem *item,Qt::Alignment align=Qt::AlignRight);
  
  Qt::Orientation orientation();
  void setOrientation(Qt::Orientation);
  
private:
  QList<QGraphicsItem*> items;
  QList<QGraphicsItemAnimation*> animations;
  
  Qt::Orientation _orientation;

  Qt::Alignment _verticalAlignment;
  Qt::Alignment _horizontalAlignment;

  void calculatePositions();
};

#endif //QGRAPHICSITEMLAYOUT_H_
