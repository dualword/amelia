#include "QTimeLineProxy.h"
#include <QDebug>

QTimeLineProxy::QTimeLineProxy(QTimeLine *original, QObject *parent)
  :QTimeLine(1,parent),_timeLine(0)
{
  setTimeLine(original);
}

void QTimeLineProxy::setTimeLine(QTimeLine *timeLine)
{
  if(_timeLine!=0)
    {
      disconnect(_timeLine,SIGNAL(valueChanged(qreal)),
		 this,SLOT(handleValueChanged(qreal)));
    }

  _timeLine=timeLine;

  if(_timeLine!=0)
    connect(_timeLine,SIGNAL(valueChanged(qreal)),
	    this,SLOT(handleValueChanged(qreal)));
}

QTimeLine *QTimeLineProxy::timeLine()
{
  return _timeLine;
}

QTimeLine::State QTimeLineProxy::state() const
{
  if(_timeLine!=0)
    return _timeLine->state();

  return QTimeLine::NotRunning;
}

void QTimeLineProxy::handleValueChanged(qreal time)
{
  if(_timeLine->direction()==direction())
    emit valueChanged(time);
  else
    emit valueChanged(1-time);
}
