#ifndef ASLIDYWIDGET_H_
#define ASLIDYWIDGET_H_

#define _USE_MATH_DEFINES

#include <QWidget>

#include "ADefines.h"

class LIBAMELIA_EXPORT ASlidyWidget : public QWidget
{
 public:
  ASlidyWidget(QWidget *parent=0);

  void addWidget(QWidget *wdg);

 protected:
  void moveEvent(QMoveEvent *event);
  void paintEvent(QPaintEvent *event);

 Q_OBJECT
};

#endif //ASLIDYWIDGET_H_
