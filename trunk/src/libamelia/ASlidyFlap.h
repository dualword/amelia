#ifndef ASLIDYFLAP_H_
#define ASLIDYFLAP_H_

#include <QAbstractButton>
#include <QPaintEvent>
#include <QMoveEvent>

class ASlidyFlap : public QAbstractButton
{
 public:
  ASlidyFlap(QWidget *parent=0);

  void setText(QString text);

  void attachWidget(QWidget *w);

 protected:
  void paintEvent(QPaintEvent *event);
  void moveEvent(QMoveEvent *event);

 private:
  QWidget *_widget;
};

#endif //ASLIDYFLAP_H_
