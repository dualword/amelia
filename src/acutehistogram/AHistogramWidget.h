#ifndef AHISTROGRAMWIDGET_H_
#define AHISTROGRAMWIDGET_H_

#include <QWidget>
#include <QPaintEvent>
#include <QHash>

uint qHash(const QColor &color);

class AHistogramWidget : public QWidget
{
 public:
  AHistogramWidget(int bins,float min,float max,QWidget* parent=0);

  void addData(QColor color,float data);
  void reset();
  
 protected:
  void paintEvent(QPaintEvent*);

 private:
  int _bins;
  float _min;
  float _max;
  
  QHash<QColor,int*> data;
};

#endif //AHISTROGRAMWIDGET_H_
