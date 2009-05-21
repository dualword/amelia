#ifndef AHIDEABUTTON_H_
#define AHIDEABUTTON_H_

#include <QWidget>
#include <QPixmap>
#include <QTimeLine>
#include <QMouseEvent>


class AHideAButton : public QWidget
{
public:
  AHideAButton(QWidget *parent=0);

  void setPixmap(QPixmap pixmap=QPixmap());

signals:
  void clicked();

public slots:
  void timerEvent(qreal step);

protected:
  void paintEvent(QPaintEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void enterEvent(QEvent *event);
  void leaveEvent(QEvent *event);

private:
  QPixmap _pixmap;
  qreal _opacity;
  QTimeLine _timeLine;

  Q_OBJECT
};

#endif //AHIDEABUTTON_H_
