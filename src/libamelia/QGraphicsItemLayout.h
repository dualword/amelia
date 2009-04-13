#ifndef QGRAPHICSITEMLAYOUT_H_
#define QGRAPHICSITEMLAYOUT_H_

#include <QGraphicsItemGroup>
#include <QGraphicsRectItem>
#include <QObject>

#include "QGraphicsItemTransformAnimation.h"

class QGraphicsItemLayout : public QObject, public QGraphicsItemGroup
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

  QTimeLine *timeLine();				      

public slots:
  void handleAnimationFinished();

signals:
  void layoutReady();
  
protected:
  virtual QTransform calculateTranslationForItem(int idx);

  float fullSpaceTakenUpInX();
  float fullSpaceTakenUpInY();
  QList<QGraphicsItem *> items();
  void calculatePositions();

private:
  QList<QGraphicsItem *> _items;
  QList<QGraphicsItemTransformAnimation *> animations;
  
  Qt::Orientation _orientation;

  Qt::Alignment _verticalAlignment;
  Qt::Alignment _horizontalAlignment;

  float _fullSpaceTakenUpInX,_fullSpaceTakenUpInY;
  float _padding;

  QTimeLine *_timeLine;

Q_OBJECT
};

#endif //QGRAPHICSITEMLAYOUT_H_
