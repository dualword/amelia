#include "QFaderWidget.h"

#include <QPainter>
#include <QDebug>
#include <QTimerEvent>
#include <QResizeEvent>

QFaderWidget::QFaderWidget(QWidget *parent)
  :QWidget(parent),opacity(1),timeLine(1000)
{
  recalculateSize();
  connect(&timeLine,SIGNAL(valueChanged(qreal)),
	  this,SLOT(setOpacity(qreal)));
  connect(&timeLine,SIGNAL(finished()),
	  this,SLOT(finalize()));
  
}

void QFaderWidget::setOpacity(qreal opacity)
{
  this->opacity=opacity;
  update();
}

void QFaderWidget::setParent(QWidget *parent)
{
  if(parentWidget()) parentWidget()->removeEventFilter(this);

  QWidget::setParent(parent);
  recalculateSize();
  parent->installEventFilter(this);
}

void QFaderWidget::paintEvent(QPaintEvent *event)
{
  if(opacity>=0)
    {
      QPainter painter(this);
      painter.setOpacity(opacity);
      painter.fillRect(geometry(),Qt::white);
    }

  QWidget::paintEvent(event);
}

void QFaderWidget::showEvent(QShowEvent *event)
{
  //opacity=1;
  //startTimer(30);
  timeLine.setDirection(QTimeLine::Backward);
  timeLine.start();
}

bool QFaderWidget::eventFilter(QObject *sender,QEvent *event)
{
  if(sender==parentWidget())
    {
      if(event->type()==QEvent::Resize)
	{
	  QResizeEvent *resizeEvent=(QResizeEvent*)event;
	  resize(resizeEvent->size());
	}
    }
  return false;
}

void QFaderWidget::finalize()
{
  parentWidget()->setUpdatesEnabled(false);
  hide();
  parentWidget()->setUpdatesEnabled(true);
  
  emit fadeFinished();
}

void QFaderWidget::recalculateSize()
{
  if(parentWidget()==0) return;

  move(0,0);
  resize(parentWidget()->size());
}
