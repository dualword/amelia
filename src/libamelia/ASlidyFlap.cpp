#include "ASlidyFlap.h"
#include <QPainter>
#include <QDebug>

ASlidyFlap::ASlidyFlap(QWidget *parent)
  :QAbstractButton(parent)
{
  resize(30,100);
}

void ASlidyFlap::attachWidget(QWidget *w)
{
  _widget=w;
}

void ASlidyFlap::setText(QString text)
{
  QAbstractButton::setText(text);
  QFontMetrics metrics=fontMetrics();
  int width=metrics.width(text);
  resize(30,width);
}

void ASlidyFlap::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);
  painter.rotate(-90);
  painter.drawText(QPoint(-height(),10),text());
}

void ASlidyFlap::moveEvent(QMoveEvent *event)
{
  if(_widget)
    {
      QPoint pos=event->pos();
      QPoint wPos=_widget->pos();
      _widget->move(pos.x()+width(),20);
    }
}
