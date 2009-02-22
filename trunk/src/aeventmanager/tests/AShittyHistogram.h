#ifndef ASHITTYHISTROGRAM_H_
#define ASHITTYHISTROGRAM_H_

#include <QGraphicsItem>
#include <QPainter>

class AShittyHistogram : public QGraphicsItem
{
 public:
  AShittyHistogram(int binWidth,QGraphicsItem* parent=0);
  
  QRectF boundingRect() const;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
  void addPoint(int value);
  void clear();
  
 private:
  int binWidth;
  QList<int> bars;
};

#endif //ASHITTYHISTROGRAM_H_
