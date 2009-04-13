#ifndef QGRAPHICSITEMTRANSFORMANIMATION_H_
#define QGRAPHICSITEMTRANSFORMANIMATION_H_

#include <QTransform>
#include <QList>
#include <QPair>
#include <QGraphicsItem>
#include <QTimeLine>

class QGraphicsItemTransformAnimation : public QObject
{
 public:
  QGraphicsItemTransformAnimation(QObject *parent=0);

  QTimeLine *timeLine();
  void setTimeLine(QTimeLine*);

  QGraphicsItem *item();
  void setItem(QGraphicsItem*);

  QList<QPair<qreal,QTransform> > transformList();
  void setTransformAt(qreal,QTransform);

 public slots:
  void setStep(qreal);
  
 private:
  QTimeLine *_timeLine;
  QGraphicsItem *_item;
  QList<QPair<qreal,QTransform> > _transformList;

  QTransform calculateTransform(int start,int end,qreal step);

  Q_OBJECT
};

#endif //QGRAPHICSITEMTRANSFORMANIMATION_H_
