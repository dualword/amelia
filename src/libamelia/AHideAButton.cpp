#include "AHideAButton.h"
#include <QPainter>

AHideAButton::AHideAButton(QWidget *parent)
  : QWidget(parent),_opacity(1),_timeLine(1000)
{ 
  connect(&_timeLine,SIGNAL(valueChanged(qreal)),
	  this,SLOT(timerEvent(qreal)));
  _timeLine.setDirection(QTimeLine::Backward);
  _timeLine.start();
}

void AHideAButton::setPixmap(QPixmap pixmap)
{
  _pixmap=pixmap;
}

void AHideAButton::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);

  painter.setOpacity(_opacity);

  QRect target(QPoint(0,0),size());
  painter.drawPixmap(target,_pixmap);
  painter.setOpacity(1);
}

void AHideAButton::enterEvent(QEvent *event)
{
  _timeLine.setDirection(QTimeLine::Forward);
  if(_timeLine.state()!=QTimeLine::Running)
    _timeLine.start();
}

void AHideAButton::leaveEvent(QEvent *event)
{
  _timeLine.setDirection(QTimeLine::Backward);
  if(_timeLine.state()!=QTimeLine::Running)
    _timeLine.start();
}

void AHideAButton::mouseReleaseEvent(QMouseEvent *event)
{
  if(event->button()==Qt::LeftButton)
    emit clicked();
}

void AHideAButton::timerEvent(qreal step)
{
  _opacity=(step<0.2)?0.2:step;
  update();
}
